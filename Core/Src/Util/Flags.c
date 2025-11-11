#include "FreeRTOS.h"   // ✅ Must come first
#include <stddef.h>
#include "semphr.h"
#include "flags.h"
#include "semphr.h"

// Private static singleton instance
static Flags flags_instance = {0};

// Private mutex
static SemaphoreHandle_t flags_mutex = NULL;

// Return pointer to singleton
Flags* get_flags(void) {
    return &flags_instance;
}

// Return mutex handle
SemaphoreHandle_t get_flags_mutex(void) {
    return flags_mutex;
}

// Initialize the flags system
void flags_init(void) {
    // Create mutex
    flags_mutex = xSemaphoreCreateMutex();
    if (flags_mutex == NULL) {
        // Handle failure (out of memory)
        // Could assert or halt
    }

    // Initialize all flags to false
    flags_instance.broadcasting = false;
    flags_instance.connected = false;
    flags_instance.reply_pending = false;
}
