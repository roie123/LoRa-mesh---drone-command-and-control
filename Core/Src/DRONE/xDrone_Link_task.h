//
// Created by royivri on 11/25/25.
//

#ifndef XDRONE_LINK_TASK_H
#define XDRONE_LINK_TASK_H
#include <stdint.h>
#define RC_CHANNELS 8
#define MSP_PAYLOAD_SIZE 16
#define MSP_SET_RAW_COMMANDS 200
#define MSP_FRAME_SIZE (5 + RC_CHANNELS*2 + 1)  // 22 bytes

typedef struct __attribute__((packed)) {
    uint8_t header1;        // '$'
    uint8_t header2;        // 'M'
    uint8_t direction;      // '<'
    uint8_t payload_size;   // RC_CHANNELS * 2 = 16
    uint8_t command;        // MSP_SET_RAW_RC = 200

    uint16_t channels[RC_CHANNELS];  // little-endian channel data

    uint8_t checksum;       // XOR of bytes [3..(3 + payload_size + 1)]
} MSP_RC_Frame;

void xDrone_link_task(void* args);
#endif //XDRONE_LINK_TASK_H
