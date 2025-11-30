//
// Created by royivri on 11/16/25.
//

#ifndef CONNECT_TO_NODE_TASK_H
#define CONNECT_TO_NODE_TASK_H
#include "LoRa.h"
#include "queue.h"


typedef struct {
    LoRa _LoRa;
    uint8_t node_id;




}Connect_to_node_args;



uint8_t send_connection_made_to_node(uint8_t node_id , QueueHandle_t tx_queue_handle);



#endif //CONNECT_TO_NODE_TASK_H
