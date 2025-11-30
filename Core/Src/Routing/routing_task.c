//
// Created by royivri on 11/13/25.
//

#include "../../Inc/routing_task.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "commands.h"
#include "../../Inc/Connect_to_node_task.h"
#include "General.h"
#include "id.h"
#include "../../Inc/NetworkData.h"
#include "packet.h"
#include "queue.h"
#include "../../Inc/Command_Queue.h"
#include "../../Inc/RC_Values.h"
#include "../../Inc/TX_Queue.h"

void routing_task(void *args) {
    Routing_task_args *routing_task_args = (Routing_task_args *) args;
    uint8_t received_byte_array[sizeof(MeshPacket)];
    MeshPacket pkt;
    CompressedPacket compressed_packet;
    MeshPacket packet_to_send;
    for (;;) {
        if (xQueueReceive(routing_task_args->_rx_queue_handle, received_byte_array,portMAX_DELAY) == pdPASS) {
            memcpy(&pkt, received_byte_array, sizeof(MeshPacket));

            Commands command = (Commands) pkt.payload[0];


            if (pkt.dst_id == mesh_id) {
                switch (command) {
                    case CONNECTION_ACK: {
                        remove_connection_request(pkt.src_id, routing_task_args->network_data_mutex);
                        add_connected_node(pkt.src_id, 0, 0, routing_task_args->network_data_mutex);
                        last_packets_sent_remove(pkt.src_id, pkt.msg_id);


                        continue;
                    }


                    case ACKNOWLEDGE: {
                        last_packets_sent_remove(pkt.src_id, pkt.msg_id);
                        continue;
                    }
                }


                Commands cmd_to_queue = command;
                xQueueSend(command_queue, &cmd_to_queue, pdMS_TO_TICKS(20));

                uint8_t ack_payload[1] = {ACKNOWLEDGE};
                mesh_build_packet(&packet_to_send, mesh_id, pkt.src_id, 0, 0, ack_payload, sizeof(ack_payload));

                xQueueSend(routing_task_args->_tx_queue_handle, &packet_to_send, pdMS_TO_TICKS(100));





                continue;
            }


            if (pkt.dst_id == BROADCAST_ADDRESS) {
                switch (command) {
                    case CONNECT_REQUEST: {
                        //TODO : implement some security check
                        if (!is_connection_request_exist(pkt.src_id, routing_task_args->network_data_mutex)) {
                            add_connection_request(pkt.src_id, routing_task_args->network_data_mutex);
                            add_connected_node(pkt.src_id, 0, 0, routing_task_args->network_data_mutex);
                            send_connection_made_to_node(pkt.src_id, tx_Queue_handle);
                            printf("CONNECTION REQUEST FROM %x \r\n", pkt.src_id);
                        } else {
                            continue;
                        }


                        continue;
                    }

                    case PING_COMMAND: {
                        add_connected_node(pkt.src_id, 0, 0, routing_task_args->network_data_mutex);
                        continue;
                    }


                    default: continue;
                }
            }

            if (pkt.dst_id != mesh_id && pkt.dst_id != BROADCAST_ADDRESS) {
                // i need to forward this packet
                if (!find_in_last_packets(pkt.dst_id, pkt.msg_id)) {
                    compressed_packet.dst_id = pkt.dst_id;
                    compressed_packet.msg_id = pkt.msg_id;
                    memcpy(&compressed_packet.payload, pkt.payload, sizeof(pkt.payload));


                    add_received_packet(&compressed_packet);
                    pkt.max_hops--;
                    xQueueSend(routing_task_args->_tx_queue_handle, &pkt, pdMS_TO_TICKS(100));
                } else {
                    continue;
                }


                continue;
            }
        }
    }
}
