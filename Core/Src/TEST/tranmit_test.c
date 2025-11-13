//
// Created by royivri on 11/13/25.
//

#include "tranmit_test.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "LoRa.h"  // your LoRa driver


void xTX_test_Task(void *args) {
    TX_TEST_Task_args *task_args = (TX_TEST_Task_args *) args;

    // Example test payload
    uint8_t test_payload[5] = {1, 2, 3, 4, 5};

    for (;;) {
        // Wait 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));

        // Send packet safely with mutex
        if (xSemaphoreTake(task_args->lora_mutex, portMAX_DELAY) == pdTRUE) {
            LoRa_transmit(task_args->lora, test_payload, sizeof(test_payload), 1000);
            xSemaphoreGive(task_args->lora_mutex);
        }
    }
}
