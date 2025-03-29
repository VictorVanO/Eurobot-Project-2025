#ifndef FSM_H
#define FSM_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Infrared.h"
#include "Encoders.h"
//#include "Servomotor.h"
#include "ButtonUrgence.h"

enum RobotState {
    IDLE,             
    FOLLOW_LINE_STATE,  
    AWAIT_OBSTACLE_STATE,
    PARTY_STATE,
    WAIT_CLEAR_STATE
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
    int motorSpeed;
    int avoidStep; 
    long ticksRotation; 
    void handleState();
    void evitementObstacle();
};

#endif