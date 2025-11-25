//
// Created by royivri on 11/25/25.
//

#ifndef RC_VALUES_H
#define RC_VALUES_H
#include <stdint.h>

#include "commands.h"

typedef  struct {
    uint16_t pitch;
    uint16_t roll;
    uint16_t yaw;
    uint16_t throttle;
    uint16_t ARM;

}Rc_values;


extern Rc_values current_rc_values;
extern Rc_values next_rc_values;

void update_rc_values(Commands command);

#endif //RC_VALUES_H
