//
// Created by royivri on 11/13/25.
//

#include "tranmit_test.h"

#include <stdio.h>

#include "commands.h"
#include "FreeRTOS.h"
#include "General.h"
#include "id.h"
#include "task.h"
#include "semphr.h"
#include "LoRa.h"  // your LoRa driver
#include "packet.h"

void xTX_test_Task(void *args) {
    TX_TEST_Task_args *task_args = (TX_TEST_Task_args *) args;

    // Example test payload
    uint8_t test_payload[1] = { CONNECT_REQUEST};
    MeshPacket packet;
    mesh_build_packet(&packet,mesh_id,BROADCAST_ADDRESS,0,0,test_payload,1);

    for (;;) {
        // Wait 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));

        // Send packet safely with mutex
        if (xSemaphoreTake(task_args->lora_mutex, portMAX_DELAY) == pdTRUE) {
            LoRa_transmit(task_args->lora, &packet, sizeof(packet), 1000);
            HAL_GPIO_TogglePin(RECEIVING_LED_GPIO_Port, RECEIVING_LED_Pin);
            xSemaphoreGive(task_args->lora_mutex);
        }
    }
}
