//
// Created by royivri on 11/11/25.
//
#include "FreeRTOS.h"
#include "packet_router.h"

#include <stdio.h>
#include <string.h>

#include "General.h"
#include "id.h"
#include "queue.h"
#include "packet.h"
#include "portmacro.h"
#include "projdefs.h"
#include "../Util/queue_implementation.h"
#include "../../Inc/commands.h"


uint8_t connected_nodes[10];
uint8_t connect_request_nodes[10];

//DEPRECATED  FROM 13/11/2025
//
//
// void packet_router_task(void *args) {
//     memset(connected_nodes, 0, sizeof(connected_nodes));
//     Packet_router_args *router_args = (Packet_router_args *) args;
//     MeshPacket received_packet; // buffer to receive a packet
//
//     MeshPacket packet_to_send;
//     memset(&packet_to_send, 0, sizeof(MeshPacket));
//
//
//     for (;;) {
//
//         if (xQueueReceive(packet_queue, &received_packet, portMAX_DELAY) == pdPASS) {
//             if (received_packet.src_id == mesh_id) {
//                 continue;
//             }
//
//             bool is_node_connected = false;
//             for (uint8_t i = 0; i < sizeof(connected_nodes); i++) {
//                 if (received_packet.src_id == connected_nodes[i]) {
//                     is_node_connected = true;
//                 }
//             }
//             Commands command = (Commands) received_packet.payload[0];
//
//
//             if (received_packet.dst_id == BROADCAST_ADDRESS) {
//                 switch (command) {
//                     case CONNECT_REQUEST: {
//                         add_node_connection_request(received_packet.src_id);
//                         printf("Connect request received from : %x\n", received_packet.src_id);
//                         uint8_t payload[1] = {(uint8_t) CONNECT_ACK};
//
//                         if (mesh_build_packet(&packet_to_send,
//                                               mesh_id,
//                                               received_packet.src_id,
//                                               FLAG_ACK_REQ,
//                                               router_args->msg_id,
//                                               payload,
//                                               1)) {
//                             uint8_t *to_byte_array = (uint8_t *) &packet_to_send;
//
//                             uint8_t to_byte_array_len = offsetof(MeshPacket, payload) + packet_to_send.length + 1;
//
//                             if (LoRa_transmit(router_args->_LoRa, to_byte_array, to_byte_array_len, 200)) {
//                                 //SUCCESS
//                             }
//                         } else {
//                             printf("cant build packet");
//                         }
//
//
//                         break;
//                     }
//
//
//                     default: break;
//                 }
//             }
//
//             if (received_packet.dst_id == mesh_id) {
//                 switch (command) {
//                     case CONNECT_ACK: {
//                         if (is_requested(received_packet.src_id)) {
//                             remove_node_connection_request(received_packet.src_id);
//                             add_node_connection(received_packet.src_id);
//
//
//                             uint8_t payload[1] = {(uint8_t) CONNECTION_MADE};
//
//                             if (mesh_build_packet(&packet_to_send,
//                                                   mesh_id,
//                                                   received_packet.src_id,
//                                                   FLAG_ACK,
//                                                   router_args->msg_id,
//                                                   payload,
//                                                   1)) {
//                                 uint8_t *to_byte_array = (uint8_t *) &packet_to_send;
//
//                                 uint8_t to_byte_array_len = offsetof(MeshPacket, payload) + packet_to_send.length + 1;
//
//                                 if (LoRa_transmit(router_args->_LoRa, to_byte_array, to_byte_array_len, 200)) {
//                                     printf("CONNECTION_MADE WITH : %x ", received_packet.src_id);
//                                     //SUCCESS
//                                 }
//                             }
//
//
//                             break;
//                         }
//
//
//                     default: break;
//                     }
//                     case CONNECTION_MADE: {
//                         remove_node_connection_request(received_packet.src_id);
//                         add_node_connection(received_packet.src_id);
//                         printf("CONNECTION_MADE WITH : %x ", received_packet.src_id);
//
//
//                         break;
//                     }
//                 }
//             }
//         }
//     }
// }
//
// bool add_node_connection_request(uint8_t node_id) {
//     for (int i = 0; i < sizeof(connect_request_nodes); i++) {
//         if (connect_request_nodes[i] == 0) {
//             connect_request_nodes[i] = node_id;
//             return true;
//         }
//     }
//     return false;
// }
//
// bool remove_node_connection_request(uint8_t node_id) {
//     for (int i = 0; i < sizeof(connect_request_nodes); i++) {
//         if (connect_request_nodes[i] == node_id) {
//             connect_request_nodes[i] = 0;
//             return true;
//         }
//     }
//     return false;
// }
//
// bool add_node_connection(uint8_t node_id) {
//     for (int i = 0; i < sizeof(connected_nodes); i++) {
//         if (connected_nodes[i] == 0) {
//             connected_nodes[i] = node_id;
//             return true;
//         }
//     }
//     return false;
// }
//
// bool is_requested(uint8_t node_id) {
//     for (int i = 0; i < sizeof(connect_request_nodes); ++i) {
//         if (connect_request_nodes[i] == node_id) {
//             return true;
//         }
//     }
//     return false;
// }
