//
// Created by royivri on 11/25/25.
//

#include "xDrone_Link_task.h"

#include <string.h>

#include "Command_Queue.h"
#include "flags.h"
#include "FreeRTOS.h"
#include "main.h"
#include "task.h"
#include "RC_Values.h"
#include "stm32f1xx_hal_uart.h"
#define MAX_COMMAND_TIME_COUNTER 15
static MSP_RC_Frame msp_rc_frame = {
    .header1 = '$',
    .header2 = 'M',
    .direction = '<',
    .payload_size = MSP_PAYLOAD_SIZE,
    .command = MSP_SET_RAW_COMMANDS,
    .channels = {0}
};
//msp_rc_frame.channels[0]   == ROLL
//msp_rc_frame.channels[1]   == PITCH
//msp_rc_frame.channels[2]   == THROTTLE
//msp_rc_frame.channels[3]   == YAW
//msp_rc_frame.channels[4]   == ARM == AUX1

extern UART_HandleTypeDef huart2;

void xDrone_link_task(void *args) {
    Commands current_command = STABLE;
    static uint8_t counter = MAX_COMMAND_TIME_COUNTER;

    for (;;) {
        if (counter==0) {
            current_command = STABLE;
            counter = MAX_COMMAND_TIME_COUNTER;
        }
       if (xQueueReceive(command_queue, &current_command, pdMS_TO_TICKS(2))) {
           counter= MAX_COMMAND_TIME_COUNTER;
       }



        switch (current_command) {
            case STABLE: {
                current_rc_values.pitch = 1500;
                current_rc_values.roll = 1500;
                current_rc_values.yaw = 1500;
                current_rc_values.throttle = 1500;
                break;
            }
            case MOVE_FORWARD: {
                current_rc_values.pitch = 1600;
                current_rc_values.throttle = 1600;
                current_rc_values.roll = 1500;
                current_rc_values.yaw = 1500;


                break;
            }

        }



        msp_rc_frame.channels[0]= current_rc_values.roll;
        msp_rc_frame.channels[1]= current_rc_values.pitch;
        msp_rc_frame.channels[2] = current_rc_values.throttle;
        msp_rc_frame.channels[3] = current_rc_values.yaw;
        msp_rc_frame.channels[4] = 1500;
        uint8_t *raw = (uint8_t *) &msp_rc_frame;

        uint8_t checksum = 0;

        // XOR payload_size + command + payload bytes
        for (uint8_t i = 3; i < 5 + msp_rc_frame.payload_size; i++)
            checksum ^= raw[i];

        msp_rc_frame.checksum = checksum;


        if (!dma_busy) {
            static uint8_t buffer[sizeof(MSP_RC_Frame)];
            memcpy(buffer, &msp_rc_frame, sizeof(MSP_RC_Frame));
            dma_busy = true;
            HAL_UART_Transmit_DMA(&huart2, buffer, sizeof(MSP_RC_Frame));
            counter--;
        }
        vTaskDelay(pdMS_TO_TICKS(200)); // 200Hz updates
    }
}
