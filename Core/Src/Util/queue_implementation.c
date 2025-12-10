//
// Created by royivri on 11/11/25.
//
#include "FreeRTOS.h"
#include "../../Inc/queue_implementation.h"

#include <stddef.h>

#include "Logger.h"
#include "packet.h"
#include "queue.h"
QueueHandle_t packet_queue = NULL;
void init_packet_queue(void) {
    // Queue can hold 10 packets at a time
    packet_queue = xQueueCreate(10, sizeof(MeshPacket));

    if(packet_queue == NULL) {
        log(FATAL,SYSTEM,"Could not create command Queue ");

    }
}

