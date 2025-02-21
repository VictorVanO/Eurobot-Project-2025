#ifndef FSM_H
#define FSM_H

#include "Motors.h"
#include "Ultrasonic.h"

#define NUM_ULTRASONIC 3  // Number of ultrasonic sensors

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
    AVOID_OBSTACLE
};

class FSM {
public:
    FSM();
    void init();
    void run();
    
private:
    RobotState state;
    RobotState previousState;
    unsigned long startTime;
    unsigned long obstacle_treshold;

    // Sensor pins as class variables
    const int trigPins[NUM_ULTRASONIC] = {22, 26, 30};  
    const int echoPins[NUM_ULTRASONIC] = {24, 28, 32};  

    bool isObstacleDetected();
    void handleState();
};

#endif