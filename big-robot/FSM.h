#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include "Motors.h"
#include "Ultrasonic.h"
#include "LCD.h"
#include "ServoArms.h"
#include "Encoders.h"
#include "Motion.h"

enum RobotState {
    INIT,
    MOVE_TO_FIRST,
    TESTS_STATE,
    DROP_BANNER,
    GO_HOME,
    PAUSE,
    AVOID_OBSTACLE
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

    unsigned long startTime;
    unsigned long obstacle_treshold;
    bool isYellow;


    bool isMoving;
    bool isMovingBackward;
    unsigned long moveStartTime;
    unsigned long moveDuration;
    bool armsFullyExtended;

    LCD* lcd;
    ServoArms* arms;

    bool isObstacleDetected();
    void handleState();
    void startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration, RobotState next);
};

#endif