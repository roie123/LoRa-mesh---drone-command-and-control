//
// Created by royivri on 11/13/25.
//

#include "../../Inc/TX_Queue.h"

#include <stdio.h>

#include "packet.h"

QueueHandle_t tx_Queue_handle = NULL; // ✅ define it exactly once

/**
 * @brief Initializes the transmit queue for outgoing MeshPackets.
 *
 * Creates a FreeRTOS queue capable of holding up to 10 MeshPacket objects.
 * The queue is used by tasks to enqueue packets for transmission over LoRa.
 *
 * @return Handle to the created queue, or NULL if creation failed.
 */

QueueHandle_t TX_Queue_init(void) {
    tx_Queue_handle = xQueueCreate(10, sizeof(MeshPacket));
    if (!tx_Queue_handle) {
        printf("TX Queue creation failed\n");
    }
    return tx_Queue_handle;
}
