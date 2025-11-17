//
// Created by royivri on 11/13/25.
//

#include "routing_task.h"

#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "Connect_to_node_task.h"
#include "General.h"
#include "id.h"
#include "NetworkData.h"
#include "packet.h"
#include "queue.h"
#include "../TX/TX_Queue.h"

void routing_task(void *args) {
    Routing_task_args *routing_task_args = (Routing_task_args *) args;
    uint8_t received_byte_array[sizeof(MeshPacket)];
    MeshPacket pkt;

    for (;;) {
        if (xQueueReceive(routing_task_args->_rx_queue_handle, received_byte_array,portMAX_DELAY) == pdPASS) {
            memcpy(&pkt, received_byte_array, sizeof(MeshPacket));
            //TODO : get rid of this line ( just for structure phase)
            printf("ROUTER : sending to TX_QUEUE \r\n");



            Commands command = (Commands) pkt.payload[0];
            // xQueueSend(routing_task_args->_tx_queue_handle,&pkt,portMAX_DELAY);





            if (pkt.dst_id == mesh_id) {
                // handle_my_packets(&pkt);

                switch (command) {

                    case CONNECTION_ACK: {
                        remove_connection_request(pkt.src_id,routing_task_args->network_data_mutex);
                        add_connected_node(pkt.src_id,0,0,routing_task_args->network_data_mutex);




                        continue;
                    }



                    default: continue;
                }


                continue;

            }

            if (pkt.dst_id == BROADCAST_ADDRESS) {

                switch (command) {
                    case CONNECT_REQUEST : {
                            //TODO : implement some security check
                        if (! is_connection_request_exist(pkt.src_id,routing_task_args->network_data_mutex)) {
                            add_connection_request(pkt.src_id,routing_task_args->network_data_mutex);
                            add_connected_node(pkt.src_id,0,0,routing_task_args->network_data_mutex);
                            send_connection_made_to_node(pkt.src_id,tx_Queue_handle);
                            printf("CONNECTION REQUEST FROM %x \r\n",pkt.src_id);




                        }else {
                            continue;
                        }






                        continue;
                    }

                    case PING_COMMAND : {




                    }





                    default:continue;
                }
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


uint8_t handle_forwarding(MeshPacket *packet) {
    //TODO  : make the forwarding logic
    return 1;

}
