//
// Created by royivri on 11/17/25.
//

#include "PONG_Queue.h"


xQueueHandle pong_queue_handle = NULL;


QueueHandle_t PongQueueInit(void) {
    pong_queue_handle = xQueueCreate(10, sizeof(Ping));

    if (pong_queue_handle != NULL) {
        return 1;
    }


    return 0;
}
