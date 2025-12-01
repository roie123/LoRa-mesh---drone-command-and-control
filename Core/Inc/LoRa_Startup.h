//
// Created by royivri on 12/1/25.
//

#ifndef LORA_STARTUP_H
#define LORA_STARTUP_H
#include <stdint.h>
#include "FreeRTOS.h"
#include "LoRa.h"
#include "semphr.h"

extern LoRa myLoRa;
extern SemaphoreHandle_t lora_mutex_handle;

uint8_t LoRa_Startup();



#endif //LORA_STARTUP_H
