//
// Created by royivri on 11/16/25.
//

#ifndef NETWORKDATA_H
#define NETWORKDATA_H
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>

#include "Node.h"
#include "packet.h"
#define MAX_NODES 10
#define MAX_CONNECTIONS_REQUESTS 10
#define LAST_PACKETS_RECEIVED_MAX 10
#define LAST_PACKETS_SENT_MAX 10



extern uint8_t connection_requests[10];
extern Node connected_nodes[MAX_NODES];
extern CompressedPacket last_received_packets[LAST_PACKETS_RECEIVED_MAX];
extern uint8_t last_received_count;





extern uint8_t last_packets_sent[LAST_PACKETS_SENT_MAX];
static uint8_t fifo_start = 0;   // index of oldest packet
static uint8_t fifo_count = 0;   // number of packets stored



void add_received_packet(CompressedPacket *pkt);

bool remove_received_packet(uint8_t dst_id);
int find_in_last_packets(uint8_t dst_id, uint8_t msg_id);
 uint8_t compute_key(uint8_t dst_id, uint8_t msg_id);



int add_connection_request(uint8_t value,SemaphoreHandle_t network_data_mutex);
int remove_connection_request(uint8_t value,SemaphoreHandle_t network_data_mutex);
int is_connection_request_exist(uint8_t value,SemaphoreHandle_t network_data_mutex);



int add_connected_node(uint8_t id, uint8_t type, int rssi,SemaphoreHandle_t network_data_mutex);
int remove_node(uint8_t id,SemaphoreHandle_t network_data_mutex);
int find_node_safe(uint8_t id,SemaphoreHandle_t network_data_mutex);
int find_node(uint8_t id);
#endif //NETWORKDATA_H
