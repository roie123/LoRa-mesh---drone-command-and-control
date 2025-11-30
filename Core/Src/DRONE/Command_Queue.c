//
// Created by royivri on 11/29/25.
//

#include "../../Inc/Command_Queue.h"

#include <stdio.h>

#include "commands.h"
QueueHandle_t command_queue=NULL;
QueueHandle_t Command_Queue_init() {
command_queue = xQueueCreate(5,sizeof(Commands));




    if (command_queue != NULL) {
        return command_queue;
    }else {
        printf("Command_Queue_init failed\n");
        return NULL;
    }




}
