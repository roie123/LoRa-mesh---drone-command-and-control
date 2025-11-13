//
// Created by royivri on 11/13/25.
//
//
// Created by royivri on 11/13/25.
//


#include "FreeRTOS.h"
#include "RX_Queue.h"


#include "packet.h"
#include "queue.h"
#include "stdio.h"

QueueHandle_t rx_queue_handle = NULL;

QueueHandle_t RX_Queue_init() {
    rx_queue_handle = xQueueCreate(RX_QUEUE_LENGTH, RX_ITEM_SIZE);
    if (rx_queue_handle != NULL) {
        return rx_queue_handle;
    }else {
        printf("RX Queue creation failed : NO MEMORY \n");
        return NULL;
    }





}