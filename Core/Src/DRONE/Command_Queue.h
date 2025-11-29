//
// Created by royivri on 11/29/25.
//

#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t command_queue;



QueueHandle_t Command_Queue_init();


#endif //COMMAND_QUEUE_H
