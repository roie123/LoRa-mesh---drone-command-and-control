//
// Created by royivri on 12/1/25.
//

#include "../../Inc/LoRa_Startup.h"
#include "LoRa.h"
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
LoRa myLoRa;
SemaphoreHandle_t lora_mutex_handle;



/**
 * @brief Initializes the LoRa module and prepares it for reception.
 *
 * Creates a mutex for thread-safe access, configures GPIO and SPI settings,
 * resets and initializes the LoRa module, and starts continuous reception.
 *
 * @return 1 if initialization succeeds, 0 if it fails.
 */

uint8_t LoRa_Startup() {

    myLoRa = newLoRa();


    lora_mutex_handle = xSemaphoreCreateMutex();


    myLoRa.CS_port = NSS_GPIO_Port;
    myLoRa.CS_pin = NSS_Pin;
    myLoRa.reset_port = RESET_GPIO_Port;
    myLoRa.reset_pin = RESET_Pin;
    myLoRa.DIO0_port = DID0_GPIO_Port;
    myLoRa.DIO0_pin = DID0_Pin;
    myLoRa.power = POWER_17db;
    myLoRa.hSPIx = &hspi2;
    LoRa_reset(&myLoRa);

    if (LoRa_init(&myLoRa) !=200) {
        return 0;
    }

    LoRa_startReceiving(&myLoRa);





    return 1;
}




