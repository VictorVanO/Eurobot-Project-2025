#ifndef FSM_H
#define FSM_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Servomotor.h"
#include "ButtonUrgence.h"

enum RobotState {
    IDLE,             
    MOVE_FORWARD_STATE,  
    TURN_LEFT,
    TURN_RIGHT,
    AVOID_OBSTACLE_STATE,
    PARTY_STATE
};

class FSM {
public:
    FSM();
    void init();
    void run();

private:
    RobotState state;
    unsigned long startTime;
    unsigned long globalTimer;
    unsigned long directionTimer; 
    int motorSpeed;
    int motorSpeed1;
    int motorSpeed2;
    void handleState();
};

#endif
