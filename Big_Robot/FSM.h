#ifndef FSM_H
#define FSM_H

#include "Motors.h"
#include "Ultrasonic.h"

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
    PREVIOUS_STATE
};

class FSM {
public:
    FSM();
    void init();
    void run();
    
private:
    RobotState state;
    unsigned long startTime;
    unsigned long obstacle_treshold;
    bool isObstacleDetected();
    void handleState();
};

#endif