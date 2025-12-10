//
// Created by royivri on 12/8/25.
//

#ifndef LOGGER_H
#define LOGGER_H
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "flags.h"
#define UART_MUTEX_TIMEOUT_MS  pdMS_TO_TICKS(5000)
#define LOG_BUFFER_SIZE 128

// SEVERITY 0 - 7
#define LOG_INFO_MASK (1U << 0)
#define LOG_DEBUG_MASK (1U << 1)
#define LOG_WARNING_MASK (1U << 2)
#define LOG_FATAL_MASK (1U << 3)

//CATEGORY 8-15
#define LOG_MESH_MASK (1U << 8)
#define LOG_SYSTEM_MASK (1U << 9)
#define LOG_DRONE_MASK (1U << 10)
#define LOG_LORA_MASK (1U << 11)



extern UART_HandleTypeDef huart3;
extern SemaphoreHandle_t uart3_mutex_handle;
extern uint32_t log_control_mask;
typedef  enum {
    INFO=LOG_INFO_MASK,
    _DEBUG=LOG_DEBUG_MASK,
    WARNING=LOG_WARNING_MASK,
    FATAL=LOG_FATAL_MASK,

}LOG_SEVERITY;
typedef enum {
    MESH=LOG_MESH_MASK,
    SYSTEM=LOG_SYSTEM_MASK,
    DRONE=LOG_DRONE_MASK,
    LORA=LOG_LORA_MASK
}LOG_CATEGORY;


void logger_init();
void log(LOG_SEVERITY severity , LOG_CATEGORY category, const char* msg);
void uart3_transmit_safe(uint8_t *buffer, uint16_t len);
const char* GetSeverityString(LOG_SEVERITY severity);
const char* GetCategoryString(LOG_CATEGORY category);

#endif //LOGGER_H
