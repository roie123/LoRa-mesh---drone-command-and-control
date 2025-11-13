//
// Created by royivri on 11/13/25.
//

#ifndef TX_QUEUE_H
#define TX_QUEUE_H
#include "FreeRTOS.h"
#include "queue.h"




extern QueueHandle_t tx_Queue_handle;

QueueHandle_t TX_Queue_init(void);


#endif //TX_QUEUE_H
