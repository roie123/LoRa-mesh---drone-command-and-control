//
// Created by royivri on 11/13/25.
//

#ifndef ROUTING_TASK_H
#define ROUTING_TASK_H
#include "FreeRTOS.h"
#include "packet.h"
#include "queue.h"
#include "semphr.h"


typedef struct {
    xQueueHandle _rx_queue_handle;
    xQueueHandle _tx_queue_handle;
    xQueueHandle _pong_queue_handle;
    SemaphoreHandle_t network_data_mutex;

} Routing_task_args;


void routing_task(void *args);
void control_next_drone();
void safe_control_next_drone();
void control_myself();
void safe_control_myself();
MeshPacket* build_forward_command(MeshPacket *packet,uint8_t command);
MeshPacket* safe_build_forward_command(MeshPacket *packet,uint8_t command);
void safe_updateRSSI_in_connected_nodes(MeshPacket *packet);
void build_ack(MeshPacket *packet);
void forward_command();
uint8_t handle_my_packets(MeshPacket *packet);

uint8_t handle_broadcasts(MeshPacket *packet);

uint8_t handle_forwarding(MeshPacket *packet);
uint8_t decrement_hops(MeshPacket *packet);
#endif //ROUTING_TASK_H
