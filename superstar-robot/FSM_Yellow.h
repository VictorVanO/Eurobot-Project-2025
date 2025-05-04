#ifndef FSM_YELLOW_H
#define FSM_YELLOW_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Servomotor.h"
#include "SwitchSelector.h"
#include "FSM_common.h"
#include "PullPin.h"  

// Finite State Machine for the yellow robot configuration
class FSM_Yellow {
public:
    FSM_Yellow();              // Constructor
    void init();               // Initializes hardware and FSM state
    void run();                // Executes main FSM logic loop
    void authorizeStart();     // Triggers the FSM start when pull-pin is removed

private:
    RobotState state;            // Current state of the FSM
    bool startAuthorized = false; // Whether the robot is allowed to start
    unsigned long startTime;      // Time when FSM started
    unsigned long globalTimer;    // General-purpose timer
    unsigned long directionTimer; // Timer used for direction logic
    unsigned long obstacleStart;  // Start time of obstacle avoidance
    unsigned long obstacleTotalTime; // Total duration spent avoiding obstacle
    int motorSpeed;               // Default motor speed
    int motorSpeed1;              // Optional motor speed 1
    int motorSpeed2;              // Optional motor speed 2
    void handleState();           // Core FSM state handler
};

#endif
