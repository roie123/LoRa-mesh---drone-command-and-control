//
// Created by royivri on 11/29/25.
//

#include "../../Inc/Command_Queue.h"

#include <stdio.h>

#include "commands.h"
QueueHandle_t command_queue=NULL;
volatile uint8_t current_selected_drone = 0xff;  // 0xff / 255 value means this is the current selected drone to command
 SemaphoreHandle_t current_selected_drone_mutex_handle=NULL;

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
    current_selected_drone_mutex_handle=xSemaphoreCreateMutex();
    if (current_selected_drone_mutex_handle==NULL) {
        return NULL;
    }
command_queue = xQueueCreate(5,sizeof(Commands));
    if (command_queue != NULL) {
        return command_queue;
    }else {
        printf("Command_Queue_init failed\n");
        return NULL;
    }

}
void send_command_fromISR(Commands cmd, BaseType_t *pxHigherPriorityTaskWoken) {
    if (cmd!=SWITCH && current_selected_drone==0xff) { // its a command for me
        xQueueSendFromISR(command_queue, &cmd, pxHigherPriorityTaskWoken);
    }else { // its a command to forward to another drone or its a switch
        uint8_t byte_array_to_router[sizeof(MeshPacket)];
        byte_array_to_router[0] = MANUAL_COMMAND_IDENTIFIER;
        byte_array_to_router[1] = (uint8_t)cmd;
        xQueueSendFromISR(rx_queue_handle, &byte_array_to_router, pxHigherPriorityTaskWoken);
    }



}
