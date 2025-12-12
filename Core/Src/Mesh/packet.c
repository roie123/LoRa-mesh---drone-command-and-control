#include "packet.h"
#include <stdint.h>
#include <string.h>

/**
 * @brief Computes an 8-bit CRC for a given data buffer.
 *
 * Uses a standard CRC-8 algorithm with polynomial 0x07 to compute a checksum
 * over the specified number of bytes.
 *
 * @param data Pointer to the data buffer.
 * @param len Number of bytes to include in the CRC computation.
 *
 * @return Computed 8-bit CRC value.
 */

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
/**
 * @brief Validates the integrity of a MeshPacket using CRC.
 *
 * Computes the CRC over the packet fields and payload, and compares it to
 * the CRC stored in the packet.
 *
 * @param pkt Pointer to the MeshPacket to validate.
 *
 * @return true if the computed CRC matches the packet's CRC, false otherwise.
 */

bool validate_packet(MeshPacket *pkt) {
    uint8_t calc = mesh_crc((uint8_t *) pkt, 9 + pkt->length);
    return (calc == pkt->crc);
}
/**
 * @brief Constructs and validates a mesh network packet.
 *
 * Initializes all fields of a MeshPacket including preamble, version, source
 * and destination IDs, flags, hop counts, message ID, and payload. Computes
 * the CRC for the packet and performs basic integrity checks.
 *
 * @param pkt Pointer to the MeshPacket to build.
 * @param src Source node ID.
 * @param dst Destination node ID.
 * @param flags Packet flags.
 * @param msg_id Message ID.
 * @param payload Pointer to the payload data.
 * @param length Length of the payload (must be <= MESH_MAX_PAYLOAD).
 *
 * @return true (non-zero) if packet was successfully built and validated, false (0) otherwise.
 */

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
    pkt->retry_count=5;
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
