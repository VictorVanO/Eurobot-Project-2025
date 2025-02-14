#include "FSM.h"

FSM::FSM() : state(INIT), startTime(0), obstacle_treshold(10) {}

void FSM::init() {
    initMotors();
    initUltrasonic();
}

void FSM::run() {
    handleState();
}

// Obstacle detection 10 cm in front of the robot
bool FSM::isObstacleDetected() {
    float distance = readDistance();
    return (distance > 0 && distance < obstacle_treshold);
}

void FSM::handleState() {
    unsigned long currentTime = millis();

    // Global check for obstacle
    if (isObstacleDetected() && state != PAUSE) {
        previousState = state;  // Save current state before pausing
        state = PAUSE;
        Serial.println("Obstacle detected ! Switching to PAUSE state.");
        return;  // Stop execution and enter PAUSE state
    }

    switch (state) {
        // Init state: Start timer
        case INIT:
            if (startTime == 0){
              startTime = millis();
            }
            // Start by moving arms. (For the tests, we start by moving in MOVE_TO_FIRST state)
            state = MOVE_TO_FIRST;
            break;

        // Move Arms state: Move the arms to fully extend the banner
        case MOVE_ARMS:
            // When arms are fully extended, change state to open hands
            state = OPEN_HANDS;
            // If arms were fully extended, move them back to initial extension width. Then start moving
            state = MOVE_TO_FIRST;
            break;
            
        // Open Hands state: Open hands to release the banner on the table & move the arms back to normal extension
        case OPEN_HANDS:
            state = MOVE_ARMS;
            break;
         
        // Move To First state: Hardcoded path to the first materials
        case MOVE_TO_FIRST:
            sweepMotor(); // Run the motor sweep function
            state = GRAB_MATERIALS;
            break;
        
        // Grab Materials state: Grab the materials to build the bleachers
        case GRAB_MATERIALS:
            state = MOVE_TO_CONSTRUCTION;
            break;
        
        // Move To Construction: Move to the construction zone with materials.
        // Hardcoded path depending on position (first or second materials).
        case MOVE_TO_CONSTRUCTION:
            state = BUILD_BLEACHER;
            break;
            
        // Build Bleacher state: Move arms and hands to build a bleacher
        case BUILD_BLEACHER:
            // After first bleacher is built
            state = MOVE_TO_SECOND;
            // After second bleacher is built
            state = GO_HOME;
            break;
        
        // Move To Second state: Move to the second materials spot
        case MOVE_TO_SECOND:
            state = GRAB_MATERIALS;
            break;
            
        // Go Home state: Move to arrival zone. Pause for a few seconds when in front of PAMI.
        case GO_HOME:
            state = PAUSE;
            break;

        // PAUSE state: Robot stop moving.
        case PAUSE:
            if (!isObstacleDetected()) {
                Serial.println("No obstacle detected. Resuming previous state.");
                state = previousState;  // Go back to the last state before PAUSE
            }
            break;
            
        // Avoid Obstacle state: Avoid the obstacle, 
        case AVOID_OBSTACLE:
            break;
    }
}