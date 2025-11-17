//
// Created by royivri on 11/8/25.
//

#ifndef NODE_H
#define NODE_H
#include <stdbool.h>
#include <stdint.h>

#include "LoRa.h"





typedef struct {
    uint8_t id;
    uint8_t type;
    int rssi;
    TickType_t last_seen;
} Node;

// uint8_t connect_to_node_by_id(uint8_t node_id, uint8_t node_type);

// uint8_t connect_to_mesh(LoRa lora, uint8_t msg_id
#endif //NODE_H
