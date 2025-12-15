//
// Created by royivri on 11/25/25.
//

#include "../../Inc/xDrone_Link_task.h"

#include <string.h>

#include "../../Inc/Command_Queue.h"
#include "flags.h"
#include "FreeRTOS.h"
#include "main.h"
#include "task.h"
#include "../../Inc/RC_Values.h"
#include "stm32f1xx_hal_uart.h"
#define MAX_COMMAND_TIME_COUNTER 5
// static MSP_RC_Frame msp_rc_frame = {
//     .header1 = '$',
//     .header2 = 'M',
//     .direction = '<',
//     .payload_size = MSP_PAYLOAD_SIZE,
//     .command = MSP_SET_RAW_COMMANDS,
//     .channels = {0}
// };
//msp_rc_frame.channels[0]   == ROLL
//msp_rc_frame.channels[1]   == PITCH
//msp_rc_frame.channels[2]   == THROTTLE
//msp_rc_frame.channels[3]   == YAW
//msp_rc_frame.channels[4]   == ARM == AUX1

extern UART_HandleTypeDef huart2;
SemaphoreHandle_t dma_mutex_handle;
static bool arm_drone = true;

uint8_t Drone_link_init() {
dma_mutex_handle = xSemaphoreCreateMutex();

    if (dma_mutex_handle == NULL) {
        return 0;
    }



    return 1;
}

/**
 * @brief Generates and sends MSP RC control frames based on received commands.
 *
 * This task maintains RC channel state directly inside a persistent
 * MSP_RC_Frame structure. Commands received from the command queue
 * incrementally modify specific RC channels (e.g. pitch, throttle, AUX),
 * and the resulting RC state persists across task iterations.
 *
 * If no command is received for a specified duration, the task enters
 * STABLE mode, which explicitly resets primary flight channels to
 * safe neutral values. AUX and other channels remain unchanged unless
 * explicitly modified by a command.
 *
 * The MSP RC frame checksum is recalculated each iteration, and frames
 * are transmitted over UART using DMA when the UART is available.
 * A countdown mechanism prevents stale commands from persisting
 * indefinitely.
 *
 * @param args Unused.
 */


void xDrone_link_task(void *args) {
    Commands current_command = STABLE;
    static uint8_t counter = MAX_COMMAND_TIME_COUNTER;
    MSP_RC_Frame msp_rc_frame = {
            .header1 = '$',
            .header2 = 'M',
            .direction = '<',
            .payload_size = MSP_PAYLOAD_SIZE,
            .command = MSP_SET_RAW_COMMANDS,
            .channels = {0}
        };
    /* Initialize RC frame ONCE */
    msp_rc_frame.channels[0] = 1500; // ROLL
    msp_rc_frame.channels[1] = 1500; // PITCH
    msp_rc_frame.channels[2] = 990;  // THROTTLE
    msp_rc_frame.channels[3] = 1500; // YAW
    msp_rc_frame.channels[4] = 1000; // AUX1 (ARM)
    msp_rc_frame.channels[5] = 1005;
    msp_rc_frame.channels[6] = 1005;
    msp_rc_frame.channels[7] = 1005;

    for (;;) {

        /* Failsafe timeout */
        if (counter == 0) {
            current_command = STABLE;
            counter = MAX_COMMAND_TIME_COUNTER;
        }

        if (xQueueReceive(command_queue, &current_command, pdMS_TO_TICKS(2))) {
            counter = MAX_COMMAND_TIME_COUNTER;
        }

        switch (current_command) {

            case STABLE:
                msp_rc_frame.channels[0] = 1500;
                msp_rc_frame.channels[1] = 1500;
                msp_rc_frame.channels[2] = 990;
                msp_rc_frame.channels[3] = 1500;
                break;

            case MOVE_FORWARD:
                msp_rc_frame.channels[1] = 1600; // PITCH
                msp_rc_frame.channels[2] = 1600; // THROTTLE
                break;

            case PITCH_UP:
                if (msp_rc_frame.channels[1] < 2000)
                    msp_rc_frame.channels[1] += 10;
                break;

            case PITCH_DOWN:
                if (msp_rc_frame.channels[1] > 1000)
                    msp_rc_frame.channels[1] -= 20;
                break;

            case ARM:
                msp_rc_frame.channels[4] = 2000; // AUX1
                break;
            case INCREASE_THROTTLE: {
                if (msp_rc_frame.channels[2]<2000) {
                    msp_rc_frame.channels[2] += 10;
                }
                break;
            }
            case DECREASE_THROTTLE: {
                if (msp_rc_frame.channels[2]>980) {
                    msp_rc_frame.channels[2] -= 10;
                }
                break;
            }
            default:
                break;
        }

        /* MSP checksum */
        uint8_t *raw = (uint8_t *)&msp_rc_frame;
        uint8_t checksum = 0;

        for (uint8_t i = 3; i < 5 + msp_rc_frame.payload_size; i++)
            checksum ^= raw[i];

        msp_rc_frame.checksum = checksum;

        /* DMA send */
        if (xSemaphoreTake(dma_mutex_handle, 0)) {

            if (!uart2_dma_busy) {
                static uint8_t buffer[sizeof(MSP_RC_Frame)];
                memcpy(buffer, &msp_rc_frame, sizeof(MSP_RC_Frame));
                uart2_dma_busy = true;
                HAL_UART_Transmit_DMA(&huart2, buffer, sizeof(MSP_RC_Frame));
                counter--;
            }

            xSemaphoreGive(dma_mutex_handle);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

