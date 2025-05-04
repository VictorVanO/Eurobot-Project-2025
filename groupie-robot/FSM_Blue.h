#ifndef FSM_BLUE_H
#define FSM_BLUE_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Infrared.h"
#include "Encoders.h"
#include "PullPin.h"          // Handles the physical start trigger (formerly Tirette)
#include "FSM_common.h"       // Shared functions/states between FSMs
#include "SwitchSelector.h"   // Manages hardware switch input
#include "ServoLed.h"         // Controls servo and LED components

// Finite State Machine for the blue robot configuration
class FSM_Blue {
public:
    FSM_Blue();               // Constructor
    void init();              // Initializes FSM and hardware
    void run();               // Executes FSM logic (called repeatedly in main loop)
    void authorizeStart();    // Authorizes the FSM to start (e.g., once pull-pin is removed)

private:
    RobotState state;         // Current FSM state
    bool startAuthorized = false; // Indicates if the FSM is allowed to start
    unsigned long startTime;  // Timestamp when the FSM starts
    unsigned long globalTimer;// General-purpose timer
    int motorSpeed;           // Current motor speed setting
    int avoidStep;            // Current step in obstacle avoidance routine
    long ticksRotation;       // Encoder ticks for managing rotation
    void handleState();       // Handles state transitions and actions
    void obstacleAvoidance(); // Handles logic for avoiding obstacles
};

#endif
