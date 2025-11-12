//
// Created by royivri on 11/10/25.
//


#include "Node.h"
#include "FreeRTOS.h"   // ✅ Must come first

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "flags.h"
#include "General.h"
#include "packet.h"
#include "semphr.h"

void discover_nodes_task(void *argument) {
    DiscoverNodesTask_args *args = (DiscoverNodesTask_args *) argument;
    MeshPacket packet;

    memset(&packet, 0, sizeof(MeshPacket));
    uint8_t payload[3] = {0x00, 0x00, 0x00};
    uint8_t payload_len = sizeof(payload);


    if (mesh_build_packet(&packet, args->id,BROADCAST_ADDRESS,FLAG_ACK_REQ, args->msg_id, payload, payload_len)) {
        uint8_t *to_byte_array = (uint8_t *) &packet;

        uint8_t to_byte_array_len = offsetof(MeshPacket, payload) + packet.length + 1;
        for (;;) {
            if (LoRa_transmit(args->lora, to_byte_array, to_byte_array_len, 200)) {
                if (xSemaphoreTake(get_flags_mutex(), pdMS_TO_TICKS(100))) {
                    get_flags()->broadcasting = true;
                    xSemaphoreGive(get_flags_mutex());
                }

                HAL_GPIO_WritePin(RECEIVING_LED_GPIO_Port, RECEIVING_LED_Pin, SET);
                vTaskDelay(pdMS_TO_TICKS(100)); // LED on for 100ms
                HAL_GPIO_WritePin(RECEIVING_LED_GPIO_Port, RECEIVING_LED_Pin, RESET);

                if (xSemaphoreTake(get_flags_mutex(), pdMS_TO_TICKS(100))) {
                    get_flags()->broadcasting = false;
                    xSemaphoreGive(get_flags_mutex());
                }
                vTaskDelay(pdMS_TO_TICKS(1000));
                //STILL ON LORA TRANSMIT
                // uint8_t notified = ulTaskNotifyTake(pdTRUE,(3000)); // i am waiting for an  interrupt from ISR
                // if (notified) {// I got a packet back from another node
                // //TODO :: i need to implement a sort of router from LoRa receive, than wake this task up to connect to another node
                //
                //
                // }else { // the timeout run out
                //
                // }

            }

        }

    } else {
        printf("Failed to build packet\n");
    }
}
