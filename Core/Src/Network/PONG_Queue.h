//
// Created by royivri on 11/17/25.
//

#ifndef PING_QUEUE_H

#define PING_QUEUE_H
#include "FreeRTOS.h"
#include "queue.h"
#define MAX_PING_QUEUE_SIZE 10

extern xQueueHandle pong_queue_handle;
typedef struct {
  uint8_t src_id;
  int signal_strength;

}Ping;

QueueHandle_t PongQueueInit(void);



#endif //PING_QUEUE_H
