//
// Created by royivri on 11/13/25.
//
#include "FreeRTOS.h"

#include "../../Inc/RX_Task.h"

#include <stdio.h>

#include "main.h"
#include "packet.h"
#include "portmacro.h"
#include "projdefs.h"
#include "RX_Queue.h"
#include "semphr.h"
#include "stm32f1xx_hal_gpio.h"
#include "task.h"
#include "../Inc/LoRa.h"
#include "../../Inc/LoRa_Startup.h"


/**
 * @brief Thread-safe wrapper for LoRa_receive().
 *
 * Takes the LoRa mutex, performs a receive operation, then releases the mutex.
 * Ensures exclusive access to the LoRa driver when receiving data.
 *
 * @param lora  Pointer to LoRa driver instance.
 * @param data  Buffer to store received bytes.
 * @param length Maximum number of bytes to read.
 * @param lora_mutex_handle Mutex protecting LoRa access.
 *
 * @return Number of bytes received.
 */

uint8_t LoRa_receive_safe(LoRa *lora, uint8_t *data, uint8_t length, SemaphoreHandle_t lora_mutex_handle) {
    uint8_t bytes = 0;
    if (xSemaphoreTake(lora_mutex_handle, portMAX_DELAY) == pdTRUE) {
        bytes = LoRa_receive(lora, data, length);
        xSemaphoreGive(lora_mutex_handle);
    }
    return bytes;
}

/**
 * @brief RX task that waits for LoRa reception notifications and forwards packets.
 *
 * This task blocks until it is notified, then performs a safe LoRa receive using
 * a mutex. If a full MeshPacket is received, it pushes the raw byte array into
 * the RX queue for further processing.
 *
 * @param args Unused.
 */

void xRX_Task(void *args) {
    uint8_t received_bytes_array[sizeof(MeshPacket)];

    for (;;) {
        uint8_t notified = ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        if (notified) {
            if (LoRa_receive_safe(&myLoRa,
                received_bytes_array,
                sizeof(received_bytes_array),
                lora_mutex_handle)) {
                received_bytes_array[RSSI_INDEX]=LoRa_getRSSI(&myLoRa);
                if (xQueueSend(rx_queue_handle,received_bytes_array,pdMS_TO_TICKS(2000))==pdTRUE) {


                };



            }
        }
    }
}
