//
// Created by royivri on 11/8/25.
//

#ifndef ID_H
#define ID_H
#include <stdint.h>


#define UNIQUE_ID_BASE 0x1FFFF7E8U

extern uint16_t mesh_id;
void node_id_init(void);   // optional function to compute it

uint32_t get_unique_id_part(uint8_t index);




#endif //ID_H
