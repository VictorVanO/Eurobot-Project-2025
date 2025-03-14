#ifndef FSM_H
#define FSM_H

#include <Arduino.h>
#include "Motors.h"
#include "Ultrasonic.h"
#include "LCD.h"
#include "ServoArms.h"

// #define NUM_ULTRASONIC 3  // Number of ultrasonic sensors

enum RobotState {
    INIT,
    MOVE_ARMS,
    OPEN_HANDS,
    MOVE_TO_FIRST,
    GRAB_MATERIALS,
    MOVE_TO_CONSTRUCTION,
    BUILD_BLEACHER,
    MOVE_TO_SECOND,
    GO_HOME,
    PAUSE,
    AVOID_OBSTACLE,
    TESTS_STATE
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
    unsigned long startTime;
    unsigned long obstacle_treshold;
    bool secondIsBuilt;

    bool isMoving;
    unsigned long moveStartTime;
    unsigned long moveDuration;
    int movementStep;
    bool armsFullyExtended;

    LCD* lcd;
    ServoArms* arms;

    bool isObstacleDetected();
    void handleState();
    void startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration);
    void handleMovementCompletion();
};

#endif