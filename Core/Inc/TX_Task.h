//
// Created by royivri on 11/13/25.
//

#ifndef TX_TASK_H
#define TX_TASK_H
#include "LoRa.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct {
    LoRa *_lora;
    xQueueHandle _tx_queue_handle;
    SemaphoreHandle_t _lora_mutex_handle;
}TX_task_args;


void xTX_task(void *args);
uint8_t LoRa_transmit_safe(LoRa *lora, uint8_t *data, uint8_t length, uint16_t timeout, SemaphoreHandle_t lora_mutex_handle);


#endif //TX_TASK_H
