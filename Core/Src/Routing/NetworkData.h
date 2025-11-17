//
// Created by royivri on 11/16/25.
//

#ifndef NETWORKDATA_H
#define NETWORKDATA_H
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>

#include "Node.h"
#define MAX_NODES 10
#define MAX_CONNECTIONS_REQUESTS 10





extern uint8_t connection_requests[10];
extern Node connected_nodes[MAX_NODES];





int add_connection_request(uint8_t value,SemaphoreHandle_t network_data_mutex);
int remove_connection_request(uint8_t value,SemaphoreHandle_t network_data_mutex);
int is_connection_request_exist(uint8_t value,SemaphoreHandle_t network_data_mutex);



int add_connected_node(uint8_t id, uint8_t type, int rssi,SemaphoreHandle_t network_data_mutex);
int remove_node(uint8_t id,SemaphoreHandle_t network_data_mutex);
int find_node_safe(uint8_t id,SemaphoreHandle_t network_data_mutex);
int find_node(uint8_t id);
#endif //NETWORKDATA_H
