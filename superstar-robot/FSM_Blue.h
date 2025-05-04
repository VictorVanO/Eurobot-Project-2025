#ifndef FSM_BLUE_H
#define FSM_BLUE_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Servomotor.h"
#include "SwitchSelector.h"
#include "FSM_common.h"
#include "PullPin.h" 

// Finite State Machine for the blue robot configuration
class FSM_Blue {
public:
    FSM_Blue();               // Constructor
    void init();              // Initialize hardware components and FSM
    void run();               // Main FSM loop
    void authorizeStart();    // Called when pull-pin is removed to authorize start

private:
    RobotState state;            // Current state of the FSM
    bool startAuthorized = false; // Whether the FSM is allowed to begin
    unsigned long startTime;      // Time when FSM starts
    unsigned long globalTimer;    // General-purpose timer
    unsigned long directionTimer; // Timer used for movement logic
    unsigned long obstacleStart;  // Timestamp for obstacle detection start
    unsigned long obstacleTotalTime; // Total time lost to obstacles
    int motorSpeed;               // Base motor speed
    int motorSpeed1;              // Speed used in special turns or states
    int motorSpeed2;              // Alternate motor speed
    void handleState();           // FSM state logic handler
};

#endif
