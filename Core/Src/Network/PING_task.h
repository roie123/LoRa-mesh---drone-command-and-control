//
// Created by royivri on 11/17/25.
//

#ifndef PING_TASK_H
#define PING_TASK_H
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#define PING_TIMING_INTERVAL_MS 10000
#define MAX_NODE_TIMEOUT 10000
typedef struct {
    SemaphoreHandle_t _network_mutex_handle;
    QueueHandle_t _tx_queue_handle;
}Ping_task_args ;


void xPing_task(void *args);

#endif //PING_TASK_H
