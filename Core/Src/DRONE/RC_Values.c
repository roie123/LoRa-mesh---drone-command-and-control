//
// Created by royivri on 11/25/25.
//

#include "RC_Values.h"


Rc_values current_rc_values = {0};
Rc_values next_rc_values = {0};


void update_rc_values(Commands command) {
    switch (command) {
        case MOVE_FORWARD: {
            next_rc_values.throttle = current_rc_values.throttle + 100;
            next_rc_values.pitch = current_rc_values.pitch + 100;
            break;



        }

            //TODO : implement other commands




    }
}
