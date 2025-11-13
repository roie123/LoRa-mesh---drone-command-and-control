//
// Created by royivri on 11/13/25.
//
#include "FreeRTOS.h"

#include "RX_Task.h"

#include <stdio.h>

#include "main.h"
#include "packet.h"
#include "portmacro.h"
#include "projdefs.h"
#include "semphr.h"
#include "stm32f1xx_hal_gpio.h"
#include "task.h"
#include "../Inc/LoRa.h"

uint8_t LoRa_receive_safe(LoRa *lora, uint8_t *data, uint8_t length, SemaphoreHandle_t lora_mutex_handle) {
    uint8_t bytes = 0;
    if (xSemaphoreTake(lora_mutex_handle, portMAX_DELAY) == pdTRUE) {
        bytes = LoRa_receive(lora, data, length);
        xSemaphoreGive(lora_mutex_handle);
    }
    return bytes;
}

void xRX_Task(void *args) {
    RX_Task_args *task_args = (RX_Task_args *) args;
    uint8_t received_bytes_array[sizeof(MeshPacket)];

    for (;;) {
        uint8_t notified = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        if (notified) {
            if (LoRa_receive_safe(task_args->lora,
                received_bytes_array,
                sizeof(received_bytes_array),
                task_args->lora_mutex)) {
                if (xQueueSend(task_args->_rx_queue_handle,received_bytes_array,pdMS_TO_TICKS(2000))==pdTRUE) {
                    printf("SENT TO RX QUEUE");
                };



            }
        }
    }
}
