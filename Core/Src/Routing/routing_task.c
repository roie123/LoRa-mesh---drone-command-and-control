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
#include "RX_Queue.h"
#include "../../Inc/Command_Queue.h"
#include "../../Inc/RC_Values.h"
#include "../../Inc/TX_Queue.h"





/**
 * @brief Routes incoming mesh packets and performs local handling.
 *
 * This FreeRTOS task waits for packets from the RX queue, parses them, and:
 *  - Handles packets addressed to this node (process command, update tables, send ACK).
 *  - Handles broadcast packets (connection requests, ping updates).
 *  - Forwards packets not meant for this node, with duplicate filtering and hop decrement.
 *
 * Ensures safe access to shared network data and reliable delivery (ACK + history tracking).
 *
 * @param args Unused.
 */
void routing_task(void *args) {
    uint8_t received_byte_array[sizeof(MeshPacket)];
    MeshPacket pkt;
    CompressedPacket compressed_packet;
    MeshPacket packet_to_send;
    for (;;) {
        if (xQueueReceive(rx_queue_handle, received_byte_array,portMAX_DELAY) == pdPASS) {

            if (received_byte_array[0]==MANUAL_COMMAND_IDENTIFIER) { //MANUAL DRONE CONTROL
                Commands cmd = received_byte_array[1];
                switch (cmd) {
                    case SWITCH : {
                        if (current_selected_drone==0xff) {
                            current_selected_drone=0;
                            continue;
                        }
                        uint8_t temp = current_selected_drone;
                        if (xSemaphoreTake(network_data_mutex_handle,10)==pdPASS) {

                            for (int i = current_selected_drone; i < MAX_NODES; ++i) {
                                if (connected_nodes[i].id!=0) {
                                    current_selected_drone=i;
                                    break;
                                }

                            }
                            xSemaphoreGive(network_data_mutex_handle);
                            if (temp==current_selected_drone) {
                                current_selected_drone=0xff;
                            }

                            continue;
                        }

                        break;

                    }



                        default: {
                        uint8_t packet_payload[1]={received_byte_array[1]}; // this is the command

                        xSemaphoreTake(network_data_mutex_handle,10);
                        uint8_t temp_address = connected_nodes[current_selected_drone].id;
                        mesh_build_packet(&packet_to_send,
                            mesh_id,
                            temp_address,
                            0,0,packet_payload,sizeof(packet_payload));

                        xSemaphoreGive(network_data_mutex_handle);
                        xQueueSend(tx_Queue_handle,&packet_to_send,10);
                    }





                }


                continue;
            }
            memcpy(&pkt, received_byte_array, sizeof(MeshPacket));
            Commands command = (Commands) pkt.payload[0];


            if (pkt.dst_id == (uint8_t)mesh_id) {
                switch (command) {
                    case CONNECTION_ACK: {
                        remove_connection_request(pkt.src_id,network_data_mutex_handle);
                        add_connected_node(pkt.src_id, 0, 0, network_data_mutex_handle);
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

                xQueueSend(tx_Queue_handle, &packet_to_send, pdMS_TO_TICKS(100));





                continue;
            }


            if (pkt.dst_id == BROADCAST_ADDRESS) {
                switch (command) {
                    case CONNECT_REQUEST: {
                        //TODO : implement some security check
                        if (!is_connection_request_exist(pkt.src_id, network_data_mutex_handle)) {
                            add_connection_request(pkt.src_id, network_data_mutex_handle);
                            add_connected_node(pkt.src_id, 0, 0, network_data_mutex_handle);
                            send_connection_made_to_node(pkt.src_id, tx_Queue_handle);
                        } else {
                            continue;
                        }


                        continue;
                    }

                    case PING_COMMAND: {
                        add_connected_node(pkt.src_id, 0, 0, network_data_mutex_handle);
                        continue;
                    }


                    default: continue;
                }
            }

            if (pkt.dst_id != mesh_id && pkt.dst_id != BROADCAST_ADDRESS) {
                // i need to forward this packet
                if (!find_in_last_packets(pkt.dst_id, pkt.msg_id)) {// i check if i didnt already got this packet
                    compressed_packet.dst_id = pkt.dst_id;
                    compressed_packet.msg_id = pkt.msg_id;
                    memcpy(&compressed_packet.payload, pkt.payload, sizeof(pkt.payload));


                    add_received_packet(&compressed_packet);
                    pkt.max_hops--;
                    xQueueSend(tx_Queue_handle, &pkt, pdMS_TO_TICKS(100)); //forwarding
                } else {
                    continue;
                }


                continue;
            }
        }
    }
}
