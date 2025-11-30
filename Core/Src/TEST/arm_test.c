//
// STM32 MSP RC sender for Betaflight
// Created for bench testing: sends 8 channels over UART6
//

#include <string.h>
#include "stm32f1xx_hal.h"
#include "../../Inc/xDrone_Link_task.h"
extern UART_HandleTypeDef huart2;   // UART connected to FC




uint16_t rc_channels[RC_CHANNELS];

// --- Mapping helper (optional) ---
uint16_t map(uint16_t value, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// --- Send MSP_SET_RAW_RC frame ---
void MSP_SendRC()
{
    uint8_t frame[MSP_FRAME_SIZE];
    memset(frame, 0, MSP_FRAME_SIZE);

    // header
    frame[0] = '$';
    frame[1] = 'M';
    frame[2] = '<';
    frame[3] = RC_CHANNELS * 2;   // payload size
    frame[4] = MSP_SET_RAW_COMMANDS;    // command

    // payload: 8 channels, little endian
    for (uint8_t ch = 0; ch < RC_CHANNELS; ch++)
    {
        frame[5 + ch*2] = rc_channels[ch] & 0xFF;
        frame[6 + ch*2] = rc_channels[ch] >> 8;
    }

    // checksum = XOR of length + command + payload
    uint8_t checksum = 0;
    for (uint8_t i = 3; i < 5 + RC_CHANNELS*2; i++)
        checksum ^= frame[i];
    frame[MSP_FRAME_SIZE - 1] = checksum;

    HAL_UART_Transmit(&huart2, frame, MSP_FRAME_SIZE, HAL_MAX_DELAY);
}

// --- Setup default RC values ---
void setupRC()
{
    for (int i = 0; i < RC_CHANNELS; i++)
        rc_channels[i] = 900;           // mid-stick default

    rc_channels[2] = 900;                // throttle low
    rc_channels[4] = 900;                // arm switch

    MSP_SendRC();
}

// --- Example loop: sweep roll, toggle arm, ramp throttle ---
void updateRCLoop()
{
    static int roll_dir = 1;
    static int pitch_dir = 1;
    static int yaw_dir = 1;

    uint32_t now = HAL_GetTick();
    static uint32_t lastAuxToggle = 0;
    rc_channels[4] = 1800;

    // --- Sweep roll (CH1) 1300 ↔ 1700
    rc_channels[0] += roll_dir * 10;
    if (rc_channels[0] >= 1700) roll_dir = -1;
    if (rc_channels[0] <= 1300) roll_dir = 1;

    // --- Sweep pitch (CH2) 1300 ↔ 1700
    rc_channels[1] += pitch_dir * 10;
    if (rc_channels[1] >= 1700) pitch_dir = -1;
    if (rc_channels[1] <= 1300) pitch_dir = 1;

    // --- Sweep yaw (CH4) 1300 ↔ 1700
    rc_channels[3] += yaw_dir * 10;
    if (rc_channels[3] >= 1700) yaw_dir = -1;
    if (rc_channels[3] <= 1300) yaw_dir = 1;

    // --- Throttle ramp (CH3) 1075 → 1500
    static uint16_t throttle = 1075;
    throttle += 1;
    if (throttle > 1500) throttle = 1075;
    rc_channels[2] = throttle;

    // --- Optional: toggle CH6 AUX every 1 second
    static uint32_t lastAux6 = 0;


    // --- Send RC frame over MSP
    MSP_SendRC();
}



