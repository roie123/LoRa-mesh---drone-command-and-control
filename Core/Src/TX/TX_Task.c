//
// Created by royivri on 11/13/25.
//

#include "../../Inc/TX_Task.h"

#include <stdio.h>
#include <string.h>

#include "Logger.h"
#include "packet.h"
#include "TX_Queue.h"
#include "../../Inc/NetworkData.h"
#include "../../Inc/LoRa_Startup.h"
/**
 * @brief Thread-safe wrapper for LoRa_transmit().
 *
 * Acquires the LoRa mutex, transmits the given data buffer, then releases the
 * mutex. Ensures exclusive access to the LoRa radio during transmission.
 *
 * @param lora   Pointer to the LoRa driver instance.
 * @param data   Buffer to transmit.
 * @param length Number of bytes to send.
 * @param timeout Transmission timeout (ms).
 * @param lora_mutex_handle Mutex protecting LoRa access.
 *
 * @return Transmission status (non-zero on success).
 */

uint8_t LoRa_transmit_safe(LoRa *lora, uint8_t *data, uint8_t length, uint16_t timeout,
                           SemaphoreHandle_t lora_mutex_handle) {
    uint8_t status = 0;


    if (xSemaphoreTake(lora_mutex_handle, portMAX_DELAY) == pdTRUE) {
        status = LoRa_transmit(lora, data, length, timeout);
        xSemaphoreGive(lora_mutex_handle);
    }

    return status;
}

/**
 * @brief Transmit task that sends mesh packets over LoRa.
 *
 * Waits for outgoing MeshPacket objects from the TX queue, assigns a global
 * message ID, and transmits them using a thread-safe LoRa send function.
 * Successfully transmitted packets are stored in the "last sent" list for
 * acknowledgment tracking and deduplication.
 *
 * @param args Unused.
 */

void xTX_task(void *args) {
    MeshPacket packet_from_queue;
    CompressedPacket compressed_packet_from_queue;

    for (;;) {
        if (xQueueReceive(tx_Queue_handle, &packet_from_queue,portMAX_DELAY) == pdTRUE) {
            packet_from_queue.msg_id = get_global_msg_id();

            if (LoRa_transmit_safe(
                &myLoRa,
                (uint8_t *) &packet_from_queue,
                sizeof(MeshPacket),
                2000,
                lora_mutex_handle


            )) {
                uint8_t index_of_found_packet_sent = find_in_last_packets(
                    packet_from_queue.dst_id, packet_from_queue.msg_id);

                if (index_of_found_packet_sent == -1) {
                    //not in sent packets

                    compressed_packet_from_queue.msg_id = packet_from_queue.msg_id;
                    compressed_packet_from_queue.dst_id = packet_from_queue.dst_id;
                    compressed_packet_from_queue.flags = packet_from_queue.flags;
                    compressed_packet_from_queue.retries_count=packet_from_queue.retry_count;
                    compressed_packet_from_queue.payload_length = packet_from_queue.length;
                    memcpy(compressed_packet_from_queue.payload, packet_from_queue.payload,
                           sizeof(packet_from_queue.payload));

                    if (xSemaphoreTake(network_data_mutex_handle, pdMS_TO_TICKS(20))) {
                        last_packets_sent_add(&compressed_packet_from_queue);
                        xSemaphoreGive(network_data_mutex_handle);
                    }else {
                        log(WARNING,SYSTEM,"xTX task : : Could not get network mutex ");
                    }


                } else {
                    // its already in sent packets
                    if (xSemaphoreTake(network_data_mutex_handle, pdMS_TO_TICKS(20))) {
                        last_packets_sent[index_of_found_packet_sent].retries_count = packet_from_queue.retry_count-1;
                        xSemaphoreGive(network_data_mutex_handle);
                    }else {
                        log(WARNING,SYSTEM,"xTX task : : Could not get network mutex ");

                    }


                }
            }
        }
    }
}
