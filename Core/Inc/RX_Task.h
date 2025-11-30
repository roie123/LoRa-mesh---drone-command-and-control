//
// Created by royivri on 11/13/25.
//

#ifndef RX_TASK_H
#define RX_TASK_H
#include "LoRa.h"
#include "semphr.h"


typedef struct {
LoRa* lora;
    SemaphoreHandle_t lora_mutex;
    xQueueHandle _rx_queue_handle;
}RX_Task_args;
uint8_t LoRa_receive_safe(LoRa *lora, uint8_t *data, uint8_t length, SemaphoreHandle_t lora_mutex_handle);



void xRX_Task(void* args );
#endif //RX_TASK_H
