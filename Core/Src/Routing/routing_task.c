//
// Created by royivri on 11/13/25.
//

#include "routing_task.h"

#include <string.h>

#include "commands.h"
#include "General.h"
#include "id.h"
#include "packet.h"
#include "queue.h"

void routing_task(void *args) {
    Routing_task_args *routing_task_args = (Routing_task_args *) args;
    uint8_t received_byte_array[sizeof(MeshPacket)];
    MeshPacket pkt;

    for (;;) {
        if (xQueueReceive(routing_task_args->_rx_queue_handle, received_byte_array,portMAX_DELAY) == pdPASS) {
            memcpy(&pkt, received_byte_array, sizeof(MeshPacket));
            //TODO : get rid of this line ( just for structure phase)
            xQueueSend(routing_task_args->_tx_queue_handle,&pkt,portMAX_DELAY);
            if (pkt.dst_id == mesh_id) {
                handle_my_packets(&pkt);
                continue;
            }

            if (pkt.dst_id == BROADCAST_ADDRESS) {
                handle_connections(&pkt);
                continue;
            }

            if (pkt.dst_id != mesh_id && pkt.dst_id != BROADCAST_ADDRESS) {
                // i did it just for readability
                handle_forwarding(&pkt);
                continue;
            }








        }
    }
}


uint8_t handle_my_packets(MeshPacket *packet) {
    Commands command = (Commands) packet->payload[0];
    switch (packet->payload[0]) {
        //TODO transmit to drone via uart

        default: return 1;
    }
    return 1;
}

uint8_t handle_connections(MeshPacket *packet) {
    //TODO : make the routing logic for connecting

    return 1;
}
uint8_t handle_forwarding(MeshPacket *packet) {
    //TODO  : make the forwarding logic
    return 1;

}
