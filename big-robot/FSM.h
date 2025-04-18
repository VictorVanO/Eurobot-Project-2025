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

enum RobotState {
    INIT,
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

    bool zipperPulled;
    
    unsigned long startTime;
    unsigned long obstacle_treshold;
    bool isYellow;

    bool isMoving;
    bool isMovingBackward;
    unsigned long moveStartTime;
    unsigned long moveDuration;
    bool armsFullyExtended;

    bool ultrasonicEnabled;


    LCD* lcd;

    bool isObstacleDetected();
    void handleState();
    void startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration, RobotState next);
};

#endif