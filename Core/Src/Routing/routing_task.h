//
// Created by royivri on 11/13/25.
//

#ifndef ROUTING_TASK_H
#define ROUTING_TASK_H
#include "FreeRTOS.h"
#include "packet.h"
#include "queue.h"




typedef struct  {
xQueueHandle _rx_queue_handle;
    xQueueHandle _tx_queue_handle;

}Routing_task_args;



void routing_task(void*args);
uint8_t handle_my_packets(MeshPacket *packet);
uint8_t handle_connections(MeshPacket *packet);
uint8_t handle_forwarding(MeshPacket *packet);
#endif //ROUTING_TASK_H
