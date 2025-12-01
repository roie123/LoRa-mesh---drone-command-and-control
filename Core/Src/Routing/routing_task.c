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

void routing_task(void *args) {
    uint8_t received_byte_array[sizeof(MeshPacket)];
    MeshPacket pkt;
    CompressedPacket compressed_packet;
    MeshPacket packet_to_send;
    for (;;) {
        if (xQueueReceive(rx_queue_handle, received_byte_array,portMAX_DELAY) == pdPASS) {
            memcpy(&pkt, received_byte_array, sizeof(MeshPacket));

            Commands command = (Commands) pkt.payload[0];


            if (pkt.dst_id == mesh_id) {
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
                            printf("CONNECTION REQUEST FROM %x \r\n", pkt.src_id);
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
                if (!find_in_last_packets(pkt.dst_id, pkt.msg_id)) {
                    compressed_packet.dst_id = pkt.dst_id;
                    compressed_packet.msg_id = pkt.msg_id;
                    memcpy(&compressed_packet.payload, pkt.payload, sizeof(pkt.payload));


                    add_received_packet(&compressed_packet);
                    pkt.max_hops--;
                    xQueueSend(tx_Queue_handle, &pkt, pdMS_TO_TICKS(100));
                } else {
                    continue;
                }


                continue;
            }
        }
    }
}
