//
// Created by royivri on 11/11/25.
//
#include "FreeRTOS.h"
#include "lora_receive.h"

#include <stdio.h>
#include <string.h>

#include "LoRa.h"
#include "packet.h"
#include "portmacro.h"
#include "projdefs.h"
#include "queue.h"
#include "task.h"
#include "../Util/queue_implementation.h"

//
// void lora_receive_task(void *args) {
//     Lora_receive_args *lora_receive_args = (Lora_receive_args *) args;
//     uint8_t rx_buffer[32];
//     uint8_t length;
//     printf("DIDO FIRED");
//     for (;;) {
//         ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
//         length = LoRa_receive(lora_receive_args->_LoRa, rx_buffer, sizeof(rx_buffer));
//
//         if (length > 0) {
//             MeshPacket pkt;
//             pkt.length = length;
//             memcpy(pkt.payload, rx_buffer, length);
//             if (xQueueSend(packet_queue,&pkt,0)!= pdPASS) {
//                 //my queue is full
//                 //TODO : handle full queue
//
//             }
//
//
//
//         }
//
//     }
// }
