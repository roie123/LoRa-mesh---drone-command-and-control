#include "packet.h"
#include <stdint.h>
#include <string.h>


uint8_t mesh_crc(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

bool validate_packet(MeshPacket *pkt) {
    uint8_t calc = mesh_crc((uint8_t *) pkt, 9 + pkt->length);
    return (calc == pkt->crc);
}

int mesh_build_packet(MeshPacket *pkt, uint8_t src, uint8_t dst, uint8_t flags, uint8_t msg_id, const uint8_t *payload,
                       uint8_t length) {
    if (!pkt || !payload || length > MESH_MAX_PAYLOAD) return false;

    pkt->preamble = MESH_PREAMBLE;
    pkt->version = MESH_VERSION;
    pkt->flags     = flags;
    pkt->src_id    = src;
    pkt->dst_id    = dst;
    pkt->hop_count = 0;
    pkt->max_hops  = 10;
    pkt->msg_id    = msg_id;
    pkt->length    = length;
    if (payload && length > 0) {
        memcpy(pkt->payload, payload, length);
    }

    uint8_t *data = (uint8_t *)pkt;
    pkt->crc = mesh_crc(data, offsetof(MeshPacket, crc));

    uint8_t computed_crc = mesh_crc((uint8_t*)pkt, offsetof(MeshPacket, crc));
    if (computed_crc != pkt->crc)
        return false;

    // 2️⃣ Check all fields are logically valid
    if (pkt->preamble != 0xAA || pkt->version != 1)
        return false;
    if (pkt->length != length)
        return false;
    if (pkt->src_id != src || pkt->dst_id != dst)
        return false;

    // Everything OK
    return true;

}

//TODO : implement
MeshPacket process_packet(MeshPacket *pkt, uint8_t packet_size) {





    return *pkt;
}
