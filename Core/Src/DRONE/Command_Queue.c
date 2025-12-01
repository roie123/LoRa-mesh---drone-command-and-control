//
// Created by royivri on 11/29/25.
//

#include "../../Inc/Command_Queue.h"

#include <stdio.h>

#include "commands.h"
QueueHandle_t command_queue=NULL;


/**
 * @brief Initializes the command queue for incoming control commands.
 *
 * Creates a FreeRTOS queue capable of holding up to 5 Commands. Tasks can
 * enqueue commands that will be processed by the drone link task or other
 * command-handling tasks.
 *
 * @return Handle to the created queue, or NULL if creation failed.
 */

QueueHandle_t Command_Queue_init() {
command_queue = xQueueCreate(5,sizeof(Commands));
    if (command_queue != NULL) {
        return command_queue;
    }else {
        printf("Command_Queue_init failed\n");
        return NULL;
    }

}
