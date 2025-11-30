//
// Created by royivri on 11/13/25.
//

#ifndef RX_QUEUE_H
#define RX_QUEUE_H
#define RX_QUEUE_LENGTH 5
#define RX_ITEM_SIZE sizeof(MeshPacket)
#include "queue.h"

extern  QueueHandle_t rx_queue_handle;


QueueHandle_t RX_Queue_init();
#endif //RX_QUEUE_H
