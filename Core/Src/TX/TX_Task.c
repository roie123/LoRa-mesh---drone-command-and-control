//
// Created by royivri on 11/13/25.
//

#include "../../Inc/TX_Task.h"

#include <stdio.h>
#include <string.h>

#include "packet.h"
#include "../../Inc/NetworkData.h"

uint8_t LoRa_transmit_safe(LoRa *lora, uint8_t *data, uint8_t length, uint16_t timeout, SemaphoreHandle_t lora_mutex_handle) {
    uint8_t status = 0;


    if (xSemaphoreTake(lora_mutex_handle, portMAX_DELAY) == pdTRUE) {
        status = LoRa_transmit(lora, data, length, timeout);
        xSemaphoreGive(lora_mutex_handle);
    }

    return status;
}

void xTX_task(void *args) {
    TX_task_args *task_args = (TX_task_args *) args;
    MeshPacket packet_from_queue;
    CompressedPacket compressed_packet_from_queue;

    for (;;) {
        if (xQueueReceive(task_args->_tx_queue_handle,&packet_from_queue,portMAX_DELAY) == pdTRUE) {
            packet_from_queue.msg_id=get_global_msg_id();

            if (LoRa_transmit_safe(
            task_args->_lora,
            (uint8_t*)&packet_from_queue,
            sizeof(MeshPacket),
            2000,
            task_args->_lora_mutex_handle


            )) {
                compressed_packet_from_queue.msg_id=packet_from_queue.msg_id;
                compressed_packet_from_queue.dst_id=packet_from_queue.dst_id;
                compressed_packet_from_queue.flags=packet_from_queue.flags;
                memcpy(compressed_packet_from_queue.payload,packet_from_queue.payload,sizeof(packet_from_queue.payload));

                last_packets_sent_add(&compressed_packet_from_queue);



            }



        }


    }
}
