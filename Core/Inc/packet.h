#ifndef MESH_PACKET_H
#define MESH_PACKET_H

//Author : Roie Ivri 1/11/25







#include <stdint.h>
#include <stdbool.h>

#define MESH_PREAMBLE 0xAA // per suggestion, i use alt bits to help detect transitions 1010101010
#define MESH_VERSION 0x01 // if i want to update vesrion i dont need to update all the boards
#define MESH_MAX_PAYLOAD 240 //my lora limit is 250
#define MESH_CRC8_POLY 0x07



#define FLAG_ACK_REQ    (1 << 0)
#define FLAG_ACK        (1 << 1)
#define FLAG_ROUTE_DISC (1 << 2)
#define FLAG_ENCRYPTED  (1 << 3)

typedef struct __attribute__((packed)) {
    uint8_t preamble;
    uint8_t version;
    uint8_t flags;
    uint8_t src_id;
    uint8_t dst_id;
    uint8_t hop_count;
    uint8_t max_hops;
    uint8_t msg_id;
    uint8_t length;
    uint8_t payload[MESH_MAX_PAYLOAD];
    uint8_t crc;
} MeshPacket;

uint8_t mesh_crc(const uint8_t *data , uint8_t len);
bool validate_packet(MeshPacket *pkt);
int mesh_build_packet(MeshPacket *pkt, uint8_t src, uint8_t dst,
                       uint8_t flags, uint8_t msg_id,
                       const uint8_t *payload, uint8_t length);
MeshPacket process_packet(MeshPacket *pkt,uint8_t packet_size);






















#endif // MESH_PACKET_H
