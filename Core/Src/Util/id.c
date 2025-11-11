//
// Created by royivri on 11/8/25.
//

#include "id.h"
inline uint32_t get_unique_id_part(uint8_t index) {
    return *((uint32_t *)(UNIQUE_ID_BASE + (index * 4)));

}



uint16_t mesh_id;

void node_id_init(void) {
    uint32_t id0 = get_unique_id_part(0);
    uint32_t id1 = get_unique_id_part(1);
    uint32_t id2 = get_unique_id_part(2);

    mesh_id = (uint16_t)((id0 ^ id1 ^ id2) & 0xFFFF);
}