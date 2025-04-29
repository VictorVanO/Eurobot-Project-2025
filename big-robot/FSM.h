#ifndef FSM_H
#define FSM_H

#define ZIPPER_PIN 8

#include <Arduino.h>
#include "Motors.h"
#include "Ultrasonic.h"
#include "LCD.h"
#include "ServoArms.h"
#include "Encoders.h"
#include "Motion.h"
#include "ServoControl.h"

enum RobotState {
    INIT,
    DROP_BANNER,
    GO_HOME,
    PAUSE,
};

class FSM {
public:
    FSM();
    ~FSM();
    void init();
    void run();
    
private:
    RobotState state;
    RobotState previousState;
    RobotState nextState;

    bool zipperPulled;
    
    unsigned long startTime;
    unsigned long obstacle_treshold;
    bool isYellow;

    bool isMoving;
    bool isMovingBackward;

    bool ultrasonicEnabled;

    LCD* lcd;

    bool isObstacleDetected();
    void handleState();
};

#endif