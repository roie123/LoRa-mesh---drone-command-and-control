//
// Created by royivri on 11/11/25.
//

#ifndef PACKET_ROUTER_H
#define PACKET_ROUTER_H
#include "LoRa.h"


typedef  struct {
    LoRa* _LoRa;
}Packet_router_args;




void packet_router_task(void *args);







#endif //PACKET_ROUTER_H
