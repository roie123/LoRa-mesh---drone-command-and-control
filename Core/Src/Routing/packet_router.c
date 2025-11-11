//
// Created by royivri on 11/11/25.
//
#include "packet_router.h"
#include "queue.h"
#include "packet.h"
#include "portmacro.h"
#include "projdefs.h"
#include "../Util/queue_implementation.h"

void packet_router_task(void *args) {
    MeshPacket pkt;  // buffer to receive a packet


    for (;;) {//TODO : create a lora_receive and a queue and get the packet from the queue

        if (xQueueReceive(packet_queue, &pkt, portMAX_DELAY) == pdPASS) {
            // Packet received successfully
            // Process or forward the packet
            // process_packet(&pkt);
        }

    }



}
