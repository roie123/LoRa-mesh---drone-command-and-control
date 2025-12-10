//
// Created by royivri on 12/8/25.
//

#include "Logger.h"

#include <stdio.h>
#include <string.h>
SemaphoreHandle_t uart3_mutex_handle = NULL;
uint32_t log_control_mask = 0x00;

void logger_init() {
    uart3_mutex_handle = xSemaphoreCreateMutex();

    if (uart3_mutex_handle == NULL) {
        //TODO : handle error
    }
}

void log(LOG_SEVERITY severity, LOG_CATEGORY category, const char *msg) {
    const char *severity_str = GetSeverityString(severity);
    const char *category_str = GetCategoryString(category);
    char log_buffer[LOG_BUFFER_SIZE];

    uint32_t msg_mask = (uint32_t) severity | (uint32_t) category;


    if (msg_mask & log_control_mask == 0) {

        int len = snprintf(
        log_buffer,                          // Destination buffer
        LOG_BUFFER_SIZE,                     // Max size of buffer
        "[%s] [%s] %s\r\n",                  // The complete, easy-to-read format string
        severity_str,                        // Arg 1: Severity string
        category_str,                        // Arg 2: Category string
        msg                                  // Arg 3: The actual log message
    );
        uart3_transmit_safe((uint8_t*)log_buffer, (uint16_t)len);

    }
}


void uart3_transmit_safe(uint8_t *buffer, uint16_t len) {
    if (len == 0) return;

    if (xSemaphoreTake(uart3_mutex_handle, UART_MUTEX_TIMEOUT_MS) == pdTRUE) {
        uart3_dma_busy = pdTRUE;

        HAL_UART_Transmit_DMA(&huart3, buffer, len);

        while (uart3_dma_busy != pdFALSE) {
            vTaskDelay(1);
        }

        xSemaphoreGive(uart3_mutex_handle);
    }
}


int __io_putchar(int ch) {
    uint8_t data = (uint8_t) ch;

    uart3_transmit_safe(&data, 1);

    return ch;
}

int _write(int file, char *ptr, int len) {
    if (file == 1 || file == 2) {
        uart3_transmit_safe((uint8_t *) ptr, len);

        return len;
    }

    return -1;
}


const char *GetSeverityString(LOG_SEVERITY severity) {
    switch (severity) {
        case INFO:
            return "INFO";
        case _DEBUG:
            return "DEBUG";
        case WARNING:
            return "WARNING";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN SEVERITY"; // Handle unexpected or uninitialized values
    }
}


const char *GetCategoryString(LOG_CATEGORY category) {
    switch (category) {
        case MESH:
            return "MESH";
        case SYSTEM:
            return "SYSTEM";
        case DRONE:
            return "DRONE";
        case LORA:
            return "LORA";
        default:
            return "UNKNOWN CATEGORY"; // Handle unexpected values
    }
}
