//
// Created by royivri on 11/25/25.
//

#include "xDrone_Link_task.h"

#include <string.h>

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"
#include "RC_Values.h"
#include "stm32f1xx_hal_uart.h"
static MSP_RC_Frame msp_rc_frame = {
    .header1 = '$',
    .header2 = 'M',
    .direction = '<',
    .payload_size = MSP_PAYLOAD_SIZE,
    .command = MSP_SET_RAW_COMMANDS,
    .channels = {0}
};


extern UART_HandleTypeDef huart2; // UART connected to FC

void xDrone_link_task(void *args) {
    for (;;) {
        if (current_rc_values.pitch != next_rc_values.pitch) {
            current_rc_values.pitch < next_rc_values.pitch ? current_rc_values.pitch++ : current_rc_values.pitch--;
        }
        if (current_rc_values.throttle != next_rc_values.throttle) {
            current_rc_values.throttle < next_rc_values.throttle
                ? current_rc_values.throttle++
                : current_rc_values.throttle--;
        }

        msp_rc_frame.channels[0] = current_rc_values.throttle;
        msp_rc_frame.channels[1] = current_rc_values.pitch;


        uint8_t *raw = (uint8_t *) &msp_rc_frame;

        uint8_t checksum = 0;

        // XOR payload_size + command + payload bytes
        for (uint8_t i = 3; i < 5 + msp_rc_frame.payload_size; i++)
            checksum ^= raw[i];

        msp_rc_frame.checksum = checksum;

        // uint8_t buffer[sizeof(MSP_RC_Frame)];
        // memcpy(buffer, &msp_rc_frame, sizeof(MSP_RC_Frame));

        HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*)&msp_rc_frame, sizeof(MSP_RC_Frame), 100);
        if (status != HAL_OK) {
            // UART timed out or busy
            // just log or skip, do not return
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 200Hz updates
    }
}
