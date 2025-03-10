#include "FSM.h"

FSM::FSM() :    state(INIT), startTime(0), obstacle_treshold(5), secondIsBuilt(false),
                isMoving(false), moveStartTime(0), moveDuration(0), movementStep(0) {
    lcd = new LCD();
}

FSM::~FSM() {
    delete lcd;
}

void FSM::init() {
    initMotors();
    initUltrasonic();
    lcd->init();
}

void FSM::run() {
    handleState();
}

void FSM::startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration) {
    moveFunction(speed);
    moveStartTime = millis();
    moveDuration = duration;
    isMoving = true;
}

bool FSM::isObstacleDetected() {
    // Get ultrasonics' distances and check for obstacle below 10cm
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        float distance = readDistance(i);
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(distance);
        Serial.print(" cm.");
        Serial.println();

        if (distance > 0 && distance <= obstacle_treshold) {
            return true; // Obstacle detected
        }
    }
    return false; // No obstacle detected
}

void FSM::handleState() {
    unsigned long currentTime = millis();

    // Always check for obstacles, even during movements
    if (isMoving) {
        // If we're currently performing a timed movement
        if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
            // Obstacle detected during movement
            stopMotors();
            isMoving = false;
            previousState = state;
            state = PAUSE;
            Serial.println("Obstacle detected during movement! Switching to PAUSE state.");
            return;
        }
        
        // Check if movement time is complete
        if (currentTime - moveStartTime >= moveDuration) {
            // Movement complete
            stopMotors();
            isMoving = false;
            
            // Advance to the next step in multi-step movements
            handleMovementCompletion();
        } else {
            // Still moving, return to keep checking for obstacles
            return;
        }
    } else {
        // Not in a movement, regular obstacle check
        if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
            previousState = state;
            state = PAUSE;
            Serial.println("Obstacle detected! Switching to PAUSE state.");
            return;
        }
    }

    switch (state) {
        // Init state: Start timer
        case INIT:
            lcd->printLine(0, "INIT LCD");
            lcd->printLine(1, "Starting robot...");

            if (startTime == 0) startTime = millis();

            // Start by moving arms. (For the tests, we start by moving in MOVE_TO_FIRST state)
            state = MOVE_TO_FIRST;
            movementStep = 0;
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
            // Only start moving if we're not already in a movement
            if (!isMoving) {
                lcd->printLine(0, "Moving to");
                lcd->printLine(1, "first bleacher");
                startTimedMovement(moveForward, 220, 2500);
            }
            break;
        
        // Grab Materials state: Grab the materials to build the bleachers
        case GRAB_MATERIALS:
            lcd->printLine(0, "Grabbing");
            lcd->printLine(1, "Materials");
            stopMotors();
            state = MOVE_TO_CONSTRUCTION;
            movementStep = 0;
            break;
        
        // Move To Construction: Move to the construction zone with materials.
        // Hardcoded path depending on position (first or second materials).
        case MOVE_TO_CONSTRUCTION:
            // Only start the next movement if we're not already moving
            if (!isMoving) {
                lcd->clear();
                lcd->printLine(0, "Moving to");
                lcd->printLine(1, "construction");
                
                // Path from first bleacher - multi-step movement
                if (!secondIsBuilt) {
                    if (movementStep == 0) {
                        // First step - turn right
                        startTimedMovement(turnRight, 220, 3000);
                    } else if (movementStep == 1) {
                        // Second step - move forward
                        startTimedMovement(moveForward, 220, 2000);
                    }
                    // Steps are incremented in handleMovementCompletion
                } else {
                    stopMotors();
                    state = BUILD_BLEACHER;
                }
            }
            break;
            
        // Build Bleacher state: Move arms and hands to build a bleacher
        case BUILD_BLEACHER:
            lcd->printLine(0, "Building...");
            if (!secondIsBuilt) {
                // After first bleacher is built
                state = MOVE_TO_SECOND;
            } else {
                // After second bleacher is built
                state = GO_HOME;
            }
            break;
        
        // Move To Second state: Move to the second materials spot
        case MOVE_TO_SECOND:
            lcd->printLine(0, "Moving to");
            lcd->printLine(1, "second bleacher");
            turnLeft(220);
            delay(3000);
            moveForward(220);
            delay(2000);
            state = GRAB_MATERIALS;
            break;
            
        // Go Home state: Move to arrival zone. Pause for a few seconds when in front of PAMI.
        case GO_HOME:
            if (!isMoving) {
                lcd->printLine(0, "Going Home...");
                lcd->printLine(1, ":)");
                startTimedMovement(moveForward, 220, 2000);
            }
            break;

        // PAUSE state: Robot stop moving.
        case PAUSE:
            lcd->printLine(0, "In PAUSE state");
            lcd->printLine(1, "Motors stopped");
            stopMotors();
            delay(5000);
            if (!isObstacleDetected()) {
                lcd->printLine(0, "No obstacle");
                lcd->printLine(1, "Resuming prev.");
                Serial.println("No more obstacle detected. Resuming previous state.");
                state = previousState;  // Go back to the last state before PAUSE
            } else {
                state = AVOID_OBSTACLE;
            }
            break;
            
        // Avoid Obstacle state: Avoid the obstacle by turning left or right, then going back on path
        case AVOID_OBSTACLE:
            lcd->printLine(0, "Avoiding");
            lcd->printLine(1, "Obstacle...");
            
            // First back up a bit to create space
            moveBackward(200);
            delay(1000);
            stopMotors();
            
            // Turn right to try to go around
            turnRight(200);
            delay(1500);
            stopMotors();
            
            // Check if path is clear
            if (!isObstacleDetected()) {
                // Move forward to go around obstacle
                moveForward(200);
                delay(1500);
                stopMotors();
                
                // Turn left to get back on path
                turnLeft(200);
                delay(1500);
                stopMotors();
                
                // Resume previous state
                state = previousState;
            } else {
                // Try turning left instead
                turnLeft(200);
                delay(3000); // Turn more to try another direction
                stopMotors();
                
                // Stay in AVOID_OBSTACLE state and try again next iteration
            }
            break;
        case TESTS_STATE:
            // Use this state for short tests
            break;
    }
}


void FSM::handleMovementCompletion() {
    // Called when a timed movement completes
    switch (state) {
        case MOVE_TO_FIRST:
            // After moving to first position
            state = GRAB_MATERIALS;
            break;
            
        case MOVE_TO_CONSTRUCTION:
            // After multi-step movement to construction zone
            if (!secondIsBuilt) {
                movementStep++;
                if (movementStep > 1) {
                    // All steps complete, transition to next state
                    state = BUILD_BLEACHER;
                    movementStep = 0;
                }
            }
            break;
            
        case MOVE_TO_SECOND:
            // After multi-step movement to second materials
            movementStep++;
            if (movementStep > 1) {
                // All steps complete, transition to next state
                state = GRAB_MATERIALS;
                movementStep = 0;
            }
            break;
            
        case AVOID_OBSTACLE:
            // Increment step in obstacle avoidance sequence
            movementStep++;
            break;
            
        // Handle other states as needed
    }
}