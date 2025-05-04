#ifndef FSM_YELLOW_H
#define FSM_YELLOW_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Infrared.h"
#include "Encoders.h"

#include "PullPin.h"          // Handles the start trigger (formerly Tirette.h)
#include "FSM_common.h"       // Shared FSM logic (formerly FSM_commun.h)
#include "SwitchSelector.h"   // Reads hardware switch selector
#include "ServoLed.h"         // Controls LED and servo output

// Finite State Machine for the yellow robot configuration
class FSM_Yellow {
public:
    FSM_Yellow();               // Constructor
    void init();                // Initializes FSM and hardware components
    void run();                 // Main FSM execution loop
    void authorizeStart();      // Authorizes start (e.g., when pull-pin is removed)

private:
    RobotState state;           // Current FSM state
    bool startAuthorized = false; // Flag to indicate if the FSM is allowed to run
    unsigned long startTime;    // Start timestamp
    unsigned long globalTimer;  // General-purpose timer
    int motorSpeed;             // Speed of the motors
    int avoidStep;              // Step within obstacle avoidance
    long ticksRotation;         // Encoder ticks used for rotation tracking
    void handleState();         // Processes state transitions
    void obstacleAvoidance();   // Executes obstacle avoidance logic
};

#endif
