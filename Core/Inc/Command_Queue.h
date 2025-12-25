//
// Created by royivri on 11/29/25.
//

#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "commands.h"
#include "FreeRTOS.h"
#include "packet.h"
#include "queue.h"
#include "RX_Queue.h"
#include "semphr.h"

extern QueueHandle_t command_queue;
extern volatile uint8_t current_selected_drone_index;
extern SemaphoreHandle_t current_selected_drone_mutex_handle;


QueueHandle_t Command_Queue_init();


void send_command_fromISR(Commands cmd, BaseType_t *pxHigherPriorityTaskWoken);



#endif //COMMAND_QUEUE_H
