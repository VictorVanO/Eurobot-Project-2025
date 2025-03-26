#include "FSM.h"

FSM::FSM() :    state(INIT), startTime(0), obstacle_treshold(15), secondIsBuilt(false), armsFullyExtended(false),
                isMoving(false), moveStartTime(0), moveDuration(0), movementStep(0) {
    lcd = new LCD();
    arms = new ServoArms();
}

FSM::~FSM() {
    delete lcd;
    delete arms;
}

void FSM::init() {
    initMotors();
    initUltrasonic();
    lcd->init();
    arms->init();
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
        // Serial.print("Sensor ");
        // Serial.print(i);
        // Serial.print(": ");
        // Serial.print(distance);
        // Serial.print(" cm.");
        // Serial.println();

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
            Serial.println("State: INIT");
            // lcd->printLine(0, "INIT LCD");
            // lcd->printLine(1, "Starting robot...");

            if (startTime == 0) startTime = millis();

            // Start by moving arms. (For the tests, we start by moving in MOVE_TO_FIRST state)
            // state = MOVE_ARMS;
            state = MOVE_TO_FIRST;
            movementStep = 0;
            break;

        // Move Arms state: Move the arms to fully extend the banner
        case MOVE_ARMS:
            Serial.println("State: Move Arms");
            // lcd->printLine(0, "Moving");
            // lcd->printLine(1, "arms...");
            if (armsFullyExtended != true) {
                // Fully extend arms, then change state to open hands to release the banner
                arms->extendArms();
                // delay(2000);
                armsFullyExtended = true;
                state = OPEN_HANDS;
            } else {
                // If arms were fully extended, move them back to initial extension width. Then start moving
                arms->retractArms();
                state = MOVE_TO_FIRST;
            }
            break;
            
        // Open Hands state: Open hands to release the banner on the table & move the arms back to normal extension
        case OPEN_HANDS:
            Serial.println("State: Open Hands");
            // lcd->printLine(0, "Opening");
            // lcd->printLine(1, "hands...");
            arms->openHands(2);
            // delay(2000);
            state = MOVE_ARMS;
            break;
         
        // Move To First state: Hardcoded path to the first materials
        case MOVE_TO_FIRST:
            // Only start moving if we're not already in a movement
            if (!isMoving) {
                // lcd->printLine(0, "Moving to");
                // lcd->printLine(1, "first bleacher");
                startTimedMovement(moveForward, 220, 3000);
            }
            break;

        case TESTS_STATE:
            // Only start moving if we're not already in a movement
            if (!isMoving) {
                startTimedMovement(moveBackward, 220, 3000);
            }
            break;
        
        
        // Grab Materials state: Grab the materials to build the bleachers
        case GRAB_MATERIALS:
            Serial.println("State: Grab Materials");
            // lcd->printLine(0, "Grabbing");
            // lcd->printLine(1, "Materials");
            stopMotors();
            arms->grabMaterials();
            // delay(2000);
            state = MOVE_TO_CONSTRUCTION;
            movementStep = 0;
            break;
        
        // Move To Construction: Move to the construction zone with materials.
        // Hardcoded path depending on position (first or second materials).
        case MOVE_TO_CONSTRUCTION:
            // Only start the next movement if we're not already moving
            if (!isMoving) {
                lcd->clear();
                // lcd->printLine(0, "Moving to");
                // lcd->printLine(1, "construction");
                
                // Path from first bleacher - multi-step movement
                if (!secondIsBuilt) {
                    if (movementStep == 0) {
                        // First step - turn right
                        startTimedMovement(turnRight, 185, 120);
                    } else if (movementStep == 1) {
                        // Second step - move forward
                        startTimedMovement(moveForward, 185, 500);
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
            // lcd->printLine(0, "Building...");
            if (!secondIsBuilt) {
                // After first bleacher is built
                secondIsBuilt = true;
                state = MOVE_TO_SECOND;
                movementStep = 0;
            } else {
                // After second bleacher is built
                state = GO_HOME;
                movementStep = 0;
            }
            break;
        
        // Move To Second state: Move to the second materials spot
        case MOVE_TO_SECOND:
            if (!isMoving) {
                // lcd->printLine(0, "Moving to");
                // lcd->printLine(1, "second bleacher");
                
                if (movementStep == 0) {
                    startTimedMovement(turnLeft, 185, 100);
                } else if (movementStep == 1) {
                    startTimedMovement(moveForward, 185, 500);
                }
            }
            break;
            
        // Go Home state: Move to arrival zone. Pause for a few seconds when in front of PAMI.
        case GO_HOME:
            if (!isMoving) {
                // lcd->printLine(0, "Going Home...");
                // lcd->printLine(1, ":)");
                startTimedMovement(moveForward, 185, 1000);
            } else {
                stopMotors();
            }
            break;

        // PAUSE state: Robot stops moving.
        case PAUSE:
            // Serial.println("Entering PAUSE state.");
            // lcd->printLine(0, "In PAUSE state");
            // lcd->printLine(1, "Motors stopped");
            
            // Non-blocking pause check
            if (!isMoving) {
                // startTimedMovement(stopMotors, 0, 2000); // Wait for 2 seconds
                delay(1000);
            // } else if (!isMoving) {
                // Pause time complete, check for obstacles again
                if (!isObstacleDetected()) {
                    Serial.println("No obstacle detected. Resuming previous state.");
                    // state = previousState;
                    state = AVOID_OBSTACLE;
                } else {
                    // state = AVOID_OBSTACLE;
                    // movementStep = 0;
                }
            }
            break;
            
        // Avoid Obstacle state: Avoid the obstacle by turning left or right, then going back on path
        case AVOID_OBSTACLE:
            Serial.println("Entering AVOID OBSTACLE state.");
            if (!isMoving) {
                lcd->clear();
                // lcd->printLine(0, "Avoiding");
                // lcd->printLine(1, "Obstacle");
                
                if (movementStep == 0) {
                    // First back up a bit
                    startTimedMovement(moveBackward, 200, 300);
                } else if (movementStep == 1) {
                    // Turn right to try to go around
                    startTimedMovement(turnRight, 200, 100);
                } else if (movementStep == 2) {
                    // Check if path is clear
                    if (!isObstacleDetected()) {
                        // Move forward
                        startTimedMovement(moveForward, 200, 500);
                    } else {
                        // Path not clear, try turning more
                        startTimedMovement(turnRight, 200, 500);
                        // Stay in same step to try again
                        movementStep = 1;
                        return;
                    }
                } else if (movementStep == 3) {
                    // Turn left to get back on path
                    startTimedMovement(turnLeft, 200, 500);
                } else if (movementStep == 4) {
                    // Return to previous state
                    state = previousState;
                    movementStep = 0;
                }
            }
            break;
    }
}

void FSM::handleMovementCompletion() {
    // Called when a timed movement completes
    switch (state) {
        case MOVE_TO_FIRST:
            // After moving to first position
            // state = GRAB_MATERIALS;
            state = TESTS_STATE;
            break;

        case TESTS_STATE:
            state = MOVE_TO_FIRST;
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