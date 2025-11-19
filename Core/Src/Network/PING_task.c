//
// Created by royivri on 11/17/25.
//

#include "PING_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>

#include "commands.h"
#include "General.h"
#include "id.h"
#include "packet.h"
#include "../Routing/NetworkData.h"


//TODO : fix complexity

void xPing_task(void *args) {
    Ping_task_args *ping_args = (Ping_task_args *)args;
    const TickType_t timeout_ticks = pdMS_TO_TICKS(MAX_NODE_TIMEOUT);
    MeshPacket packet = {0};
    uint8_t payload[1]={PING_COMMAND};
    mesh_build_packet(&packet,mesh_id,BROADCAST_ADDRESS,0,0,payload,1);
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(PING_TIMING_INTERVAL_MS));

        TickType_t now = xTaskGetTickCount();
        if (xSemaphoreTake(ping_args->_network_mutex_handle, pdMS_TO_TICKS(100)) == pdTRUE) {
            for (int i = 0; i < MAX_NODES; ++i) {
                if (connected_nodes[i].id != 0 &&
                    (now - connected_nodes[i].last_seen > timeout_ticks)) {
                    // Node timed out
                    connected_nodes[i].id = 0;
                    connected_nodes[i].rssi = 0;
                    connected_nodes[i].type = 0;
                    connected_nodes[i].last_seen = 0;
                    }
            }
            xSemaphoreGive(ping_args->_network_mutex_handle);
        }

        BaseType_t sent = xQueueSend(ping_args->_tx_queue_handle, &packet, portMAX_DELAY);
        if (sent != pdTRUE) {
            printf("Ping task: Failed to send packet to TX queue!\n");
        }
    }
}
