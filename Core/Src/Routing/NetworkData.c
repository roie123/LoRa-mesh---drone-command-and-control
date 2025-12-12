//
// Created by royivri on 11/16/25.
//

#include "../../Inc/NetworkData.h"

#include <string.h>


//ONLY 1 task is allowed to change this array
uint8_t connection_requests[MAX_CONNECTIONS_REQUESTS] = {0};


Node connected_nodes[MAX_NODES] = {0};

CompressedPacket last_received_packets[10] = {0};
CompressedPacket last_packets_sent[LAST_PACKETS_SENT_MAX] = {0};
SemaphoreHandle_t network_data_mutex_handle;


uint8_t network_data_init(void) {
    network_data_mutex_handle = xSemaphoreCreateMutex();
    return 1;
}

int add_connection_request(uint8_t value, SemaphoreHandle_t network_data_mutex) {
    if (xSemaphoreTake(network_data_mutex, portMAX_DELAY) == pdTRUE) {
        for (int i = 0; i < MAX_CONNECTIONS_REQUESTS; i++) {
            if (connection_requests[i] == 0) {
                // first empty slot
                connection_requests[i] = value;
                xSemaphoreGive(network_data_mutex);

                return i; // return index where added
            }
        }
        xSemaphoreGive(network_data_mutex);
        return -1; // array full
    }
    return -1;
}


int remove_connection_request(uint8_t value, SemaphoreHandle_t network_data_mutex) {
    if (xSemaphoreTake(network_data_mutex, portMAX_DELAY) == pdTRUE) {
        for (int i = 0; i < MAX_CONNECTIONS_REQUESTS; i++) {
            if (connection_requests[i] == value) {
                connection_requests[i] = 0;
                xSemaphoreGive(network_data_mutex);
                return i;
            }
        }
        xSemaphoreGive(network_data_mutex);
        return -1; // not found
    }
    return -1;
}

int is_connection_request_exist(uint8_t value, SemaphoreHandle_t network_data_mutex) {
    if (xSemaphoreTake(network_data_mutex, portMAX_DELAY) == pdTRUE) {
        for (int i = 0; i < MAX_CONNECTIONS_REQUESTS; i++) {
            if (connection_requests[i] == value) {
                xSemaphoreGive(network_data_mutex);
                return 1; // exists
            }
        }
        xSemaphoreGive(network_data_mutex);

        return 0; // does not exist
    }
    return -1; //i couldnt get the mutex ;
}


int add_connected_node(uint8_t id, uint8_t type, int rssi, SemaphoreHandle_t network_data_mutex) {
    // Already exists? Update it


    if (xSemaphoreTake(network_data_mutex, portMAX_DELAY) == pdTRUE) {
        int idx = find_node(id);


        if (idx >= 0) {
            connected_nodes[idx].last_seen = xTaskGetTickCount();
            connected_nodes[idx].type = type;
            connected_nodes[idx].rssi = rssi;
            xSemaphoreGive(network_data_mutex);
            return idx;
        }

        // Find first empty slot
        for (int i = 0; i < MAX_NODES; i++) {
            if (connected_nodes[i].id == 0) {
                // empty slot
                connected_nodes[i].last_seen = xTaskGetTickCount();
                connected_nodes[i].id = id;
                connected_nodes[i].type = type;
                connected_nodes[i].rssi = rssi;
                xSemaphoreGive(network_data_mutex);
                return i;
            }
        }
        xSemaphoreGive(network_data_mutex);
    }


    return -1; // no empty slot
}

// Find a node by id. Returns index or -1 if not found
int find_node_safe(uint8_t id, SemaphoreHandle_t network_data_mutex) {
    if (xSemaphoreTake(network_data_mutex, portMAX_DELAY) == pdTRUE) {
        for (int i = 0; i < MAX_NODES; i++) {
            if (connected_nodes[i].id == id) {
                xSemaphoreGive(network_data_mutex);
                return i;
            }
        }
        xSemaphoreGive(network_data_mutex);
        return -1;
    }

    return -2;
}

int find_node(uint8_t id) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (connected_nodes[i].id == id) {
            return i;
        }
    }
    return -1;
}


// Remove a node by id. Returns index or -1 if not found
int remove_node(uint8_t id, SemaphoreHandle_t network_data_mutex) {
    int idx = find_node_safe(id, network_data_mutex);
    if (idx < 0) return -1;
    connected_nodes[idx].last_seen = xTaskGetTickCount();

    connected_nodes[idx].id = 0;
    connected_nodes[idx].type = 0;
    connected_nodes[idx].rssi = 0;

    return idx;
}

inline uint8_t compute_key(uint8_t dst_id, uint8_t msg_id) {
    return dst_id ^ msg_id;
}

int find_in_last_packets(uint8_t dst_id, uint8_t msg_id) {
    uint8_t key = compute_key(dst_id, msg_id);

    for (int i = 0; i < 10; i++) {
        if ((last_received_packets[i].dst_id ^ last_received_packets[i].msg_id) == key) {
            return i; // return index
        }
    }

    return -1; // not found
}

void remove_packet(uint8_t dst_id, uint8_t msg_id) {
    int index = find_in_last_packets(dst_id, msg_id);
    if (index < 0) return;

    // Clear entry
    memset(&last_received_packets[index], 0, sizeof(CompressedPacket));
}


uint8_t get_global_msg_id() {
    return ++global_msg_id;
}

void add_received_packet(CompressedPacket *pkt) {
    for (int i = 0; i < 10; i++) {
        if (last_received_packets[i].dst_id == 0) {
            last_received_packets[i] = *pkt;
            return;
        }
    }

    // Table full → overwrite slot 0
    last_received_packets[0] = *pkt;
}

//  ****************************************************************************************************** /

static uint8_t fifo_start_sent = 0;
static uint8_t fifo_count_sent = 0;

// Compute key
static inline uint8_t packet_key(const CompressedPacket *pkt) {
    return pkt->dst_id ^ pkt->msg_id;
}

// Add a packet (FIFO, overwrite oldest if full)
void last_packets_sent_add(const CompressedPacket *pkt) {
    if (xSemaphoreTake(network_data_mutex_handle, pdMS_TO_TICKS(10))) {
        if (fifo_count_sent < LAST_PACKETS_SENT_MAX) {
            uint8_t end = (fifo_start_sent + fifo_count_sent) % LAST_PACKETS_SENT_MAX;
            last_packets_sent[end] = *pkt;
            fifo_count_sent++;
        } else {
            // FIFO full → overwrite oldest
            last_packets_sent[fifo_start_sent] = *pkt;
            fifo_start_sent = (fifo_start_sent + 1) % LAST_PACKETS_SENT_MAX;
        }


        xSemaphoreGive(network_data_mutex_handle);
    }
}

// Find a packet by key (dst_id ^ msg_id)
CompressedPacket *last_packets_sent_find(uint8_t dst_id, uint8_t msg_id) {
    uint8_t key = dst_id ^ msg_id;
    for (uint8_t i = 0; i < fifo_count_sent; i++) {
        uint8_t idx = (fifo_start_sent + i) % LAST_PACKETS_SENT_MAX;
        if (packet_key(&last_packets_sent[idx]) == key) {
            return &last_packets_sent[idx];
        }
    }
    return NULL; // not found
}

// Remove a packet by key (dst_id ^ msg_id)
bool last_packets_sent_remove(uint8_t dst_id, uint8_t msg_id) {
    uint8_t key = dst_id ^ msg_id;
    for (uint8_t i = 0; i < fifo_count_sent; i++) {
        uint8_t idx = (fifo_start_sent + i) % LAST_PACKETS_SENT_MAX;
        if (packet_key(&last_packets_sent[idx]) == key) {
            // shift everything after it left
            for (uint8_t j = i; j < fifo_count_sent - 1; j++) {
                uint8_t from = (fifo_start_sent + j + 1) % LAST_PACKETS_SENT_MAX;
                uint8_t to = (fifo_start_sent + j) % LAST_PACKETS_SENT_MAX;
                last_packets_sent[to] = last_packets_sent[from];
            }
            fifo_count_sent--;
            return true;
        }
    }
    return false;
}
