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
    uint8_t signal_strength;
} Node;

uint8_t connect_to_node_by_id(uint8_t node_id, uint8_t node_type);

uint8_t connect_to_mesh(LoRa lora, uint8_t msg_id, uint8_t);

void discover_nodes_task(void *argument);

typedef struct {
    uint8_t id; // this drone ID
    uint8_t msg_id;
    LoRa* lora;
} DiscoverNodesTask_args;

#endif //NODE_H
