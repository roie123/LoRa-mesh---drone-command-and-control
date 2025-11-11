#ifndef FLAGS_H
#define FLAGS_H
#include "FreeRTOS.h"   // ✅ Must come first
#include <stdbool.h>

#include "semphr.h"


typedef struct {
    bool broadcasting;
    bool connected;
    bool reply_pending;
} Flags;


Flags* get_flags(void);


SemaphoreHandle_t get_flags_mutex(void);


void flags_init(void);

#endif // FLAGS_H
