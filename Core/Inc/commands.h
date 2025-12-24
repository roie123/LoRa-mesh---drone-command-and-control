//
// Created by royivri on 11/6/25.
//

#ifndef COMMANDS_H
#define COMMANDS_H
#define MANUAL_COMMAND_IDENTIFIER 0x00 //sits at the start of an incoming array
#define MANUAL_COMMAND_IDENTIFIER_INDEX 0U// the identifier for a command array
#define MANUAL_COMMAND_INDEX 1U // the index of the actual command


typedef enum {
    MOVE_FORWARD=0,
    MOVE_BACKWARD=1,
    ROTATE_LEFT=2,
    ROTATE_RIGHT=3,
    INCREASE_THROTTLE=4,
    DECREASE_THROTTLE=5,
    CONNECT_REQUEST=6,
    CONNECTION_ACK=7,
    CONNECTION_MADE=8,
    PING_COMMAND=9,
    ACKNOWLEDGE=10,
    STABLE=11,
    SWITCH=12,
    PITCH_UP=13,
    PITCH_DOWN=14,
    ARM=15


}Commands;



#endif //COMMANDS_H
