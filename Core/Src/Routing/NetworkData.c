//
// Created by royivri on 11/16/25.
//

#include "NetworkData.h"


//ONLY 1 task is allowed to change this array
uint8_t connection_requests[MAX_CONNECTIONS_REQUESTS] = {0};


Node connected_nodes[MAX_NODES] = {0};


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
            connected_nodes[idx].type = type;
            connected_nodes[idx].rssi = rssi;
            xSemaphoreGive(network_data_mutex);
            return idx;
        }

        // Find first empty slot
        for (int i = 0; i < MAX_NODES; i++) {
            if (connected_nodes[i].id == 0) {
                // empty slot
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

    connected_nodes[idx].id = 0;
    connected_nodes[idx].type = 0;
    connected_nodes[idx].rssi = 0;

    return idx;
}
