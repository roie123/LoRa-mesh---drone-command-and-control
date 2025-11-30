//
// Created by royivri on 11/13/25.
//

#ifndef TRANMIT_TEST_H
#define TRANMIT_TEST_H
#include "FreeRTOS.h"
#include "LoRa.h"
#include "semphr.h"


typedef struct {
    LoRa *lora;
    SemaphoreHandle_t lora_mutex;
} TX_TEST_Task_args;



void xTX_test_Task(void *args);
#endif //TRANMIT_TEST_H
