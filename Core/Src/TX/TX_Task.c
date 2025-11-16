//
// Created by royivri on 11/13/25.
//

#include "TX_Task.h"

#include <stdio.h>

#include "packet.h"

uint8_t LoRa_transmit_safe(LoRa *lora, uint8_t *data, uint8_t length, uint16_t timeout, SemaphoreHandle_t lora_mutex_handle) {
    uint8_t status = 0;

    // Take the LoRa mutex before using the SPI and LoRa hardware
    if (xSemaphoreTake(lora_mutex_handle, portMAX_DELAY) == pdTRUE) {
        status = LoRa_transmit(lora, data, length, timeout);
        xSemaphoreGive(lora_mutex_handle);  // Always release the mutex
    }

    return status;
}

void xTX_task(void *args) {
    TX_task_args *task_args = (TX_task_args *) args;
    MeshPacket packet_from_queue;


    for (;;) {
        if (xQueueReceive(task_args->_tx_queue_handle,&packet_from_queue,portMAX_DELAY) == pdTRUE) {

            if (LoRa_transmit_safe(
            task_args->_lora,
            packet_from_queue.payload,
            packet_from_queue.length,
            2000,
            task_args->_lora_mutex_handle


            )) {
                printf("xTX : SENT PACKET  \r\n");




            }



        }


    }
}
