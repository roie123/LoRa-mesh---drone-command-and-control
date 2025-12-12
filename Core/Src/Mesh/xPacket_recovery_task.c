//
// Created by royivri on 12/12/25.
//

#include "xPacket_recovery_task.h"

#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "id.h"
#include "Logger.h"
#include "NetworkData.h"
#include "projdefs.h"
#include "semphr.h"
#include "task.h"
#include "TX_Queue.h"

void xPacketRecoveryTask(void *pvParameters) {
    MeshPacket packet = {0};
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(PACKET_RECOVERY_SLEEP_TIME));

        if (xSemaphoreTake(network_data_mutex_handle, pdMS_TO_TICKS(50))) {
            uint32_t now = xTaskGetTickCount();
            for (int i = 0; i < LAST_PACKETS_SENT_MAX; ++i) {
                if (now - last_packets_sent[i].last_sent_time > PACKET_RECOVERY_RETRY_DELAY_MS) {
                    mesh_build_packet(&packet, mesh_id, last_packets_sent[i].dst_id,NULL, 0,
                                      last_packets_sent[i].payload, last_packets_sent[i].payload_length);
                    if (!xQueueSend(tx_Queue_handle, &packet, pdMS_TO_TICKS(50))) {
                        log(WARNING, SYSTEM, "xPacketRecoveryTask: failed to send to xTx");
                    } else {
                        last_packets_sent[i].last_sent_time = now;
                    }
                }
            }


            xSemaphoreGive(network_data_mutex_handle);
        }
    }
}
