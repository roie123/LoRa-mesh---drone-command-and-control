//
// Created by royivri on 11/16/25.
//

#include "FreeRTOS.h"
#include "../../Inc/Connect_to_node_task.h"

#include <string.h>

#include "commands.h"
#include "id.h"
#include "Node.h"
#include "packet.h"
#include "portmacro.h"
#include "queue.h"


//I turned it into a task because i dont want it to block the "packet pipeline"

uint8_t send_connection_made_to_node(uint8_t node_id,QueueHandle_t tx_queue) {
    // Connect_to_node_args *connect_to_node_args = (Connect_to_node_args *) args;
    MeshPacket packet;
    memset(&packet, 0, sizeof(packet));
    uint8_t payload[1] = {CONNECTION_MADE};
    mesh_build_packet(&packet, mesh_id,node_id,0,0,payload,1);

    if (xQueueSend(tx_queue,&packet,portMAX_DELAY)==pdTRUE) {
        return 1;

    }
return 0;


}
