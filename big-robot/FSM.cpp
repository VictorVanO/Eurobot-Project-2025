#include "FSM.h"

FSM::FSM() :    state(INIT), startTime(0), obstacle_treshold(18), secondIsBuilt(false), armsFullyExtended(false),
                moveStartTime(0), moveDuration(0), movementStep(0), startAvoidance(0) {
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
}

bool FSM::isObstacleDetected() {
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        float distance = readDistance(i);
        if (distance > 0 && distance <= obstacle_treshold) {
            return true; // Obstacle detected
        }
    }
    return false; // No obstacle detected
}

void FSM::handleState() {
    unsigned long currentTime = millis();
    static unsigned long pauseStartTime = 0;
    unsigned long turnDuration = 600; 
    const unsigned long backwardAvoidanceDuration = 600;
    const unsigned long turnRightDuration = 600;

    // Check for ongoing timed movement
    if (moveStartTime > 0) {
        // Check if movement time is complete
        if (currentTime - moveStartTime >= moveDuration) {
            stopMotors();
            moveStartTime = 0;
            handleMovementCompletion();
        }
        
        // Obstacle check during movement
        if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
            stopMotors();
            moveStartTime = 0;
            previousState = state;
            state = PAUSE;
            pauseStartTime = currentTime;
            Serial.println("Obstacle detected during movement! Switching to PAUSE state.");
            return;
        }
        
        // Continue current movement if not stopped
        return;
    }

    // Regular obstacle check when not moving
    if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
        previousState = state;
        state = PAUSE;
        pauseStartTime = currentTime;
        Serial.println("Obstacle detected! Switching to PAUSE state.");
        return;
    }

    switch (state) {
        case INIT:
            Serial.println("State: INIT");
            if (startTime == 0) startTime = millis();
            state = MOVE_TO_FIRST;
            movementStep = 0;
            break;

        case MOVE_ARMS:
            Serial.println("State: Move Arms");
            if (armsFullyExtended != true) {
                arms->extendArms(); 
                armsFullyExtended = true;
                state = OPEN_HANDS;
            } else {
                arms->retractArms();
                state = MOVE_TO_FIRST;
            }
            break;
            
        case OPEN_HANDS:
            Serial.println("State: Open Hands");
            arms->openHands(2);
            state = MOVE_ARMS;
            break;
         
        case MOVE_TO_FIRST:
            Serial.println("State: MOVE TO FIRST");
            arms->extendArms(); 
            startTimedMovement(moveForward, 220, 5000);
            break;

        case TESTS_STATE:
            Serial.println("State: TESTS");
            arms->retractArms();
            startTimedMovement(moveForward, 160, 2500);
            break;
        
        case GRAB_MATERIALS:
            Serial.println("State: Grab Materials");
            stopMotors();
            arms->grabMaterials();
            state = MOVE_TO_CONSTRUCTION;
            movementStep = 0;
            break;
        
        case MOVE_TO_CONSTRUCTION:
            if (!secondIsBuilt) {
                if (movementStep == 0) {
                    startTimedMovement(turnRight, 185, 120);
                } else if (movementStep == 1) {
                    startTimedMovement(moveForward, 185, 500);
                }
            } else {
                stopMotors();
                state = BUILD_BLEACHER;
            }
            break;
            
        case BUILD_BLEACHER:
            if (!secondIsBuilt) {
                secondIsBuilt = true;
                state = MOVE_TO_SECOND;
                movementStep = 0;
            } else {
                state = GO_HOME;
                movementStep = 0;
            }
            break;
        
        case MOVE_TO_SECOND:
            if (movementStep == 0) {
                startTimedMovement(turnLeft, 185, 100);
            } else if (movementStep == 1) {
                startTimedMovement(moveForward, 185, 500);
            }
            break;
            
        case GO_HOME:
            startTimedMovement(moveForward, 185, 1000);
            break;

        case PAUSE:
            Serial.println("State: PAUSE");
            stopMotors();
            if (currentTime - pauseStartTime >= 2000) {
                if (isObstacleDetected()) {
                    state = AVOID_OBSTACLE;
                    movementStep = 0;
                } else {
                    state = previousState;
                }
            }
            break;
            
        case AVOID_OBSTACLE:
            Serial.println("Robot state: AVOID_OBSTACLE");
            
            if (startAvoidance == 0) {
                startAvoidance = millis();
            }

            unsigned long elapsedTime = millis() - startAvoidance;

            if (elapsedTime < backwardAvoidanceDuration) {
                moveBackward(220);
            }
            else if (elapsedTime < backwardAvoidanceDuration + turnDuration) {
                turnRight(220);  // Tourner à droite pour éviter l'obstacle
            }
            else {
                if (isObstacleDetected()) {
                    // Serial.println("Obstacle toujours présent, recommence l'évitement...");
                    startAvoidance = millis(); // Réinitialiser le timer et recommencer l'évitement
                } else {
                    // Serial.println("Obstacle évité !");
                    stopMotors();
                    startAvoidance = 0;
                    state = MOVE_TO_FIRST;
                }
            }
            break;
    }
}

void FSM::handleMovementCompletion() {
    switch (state) {
        case MOVE_TO_FIRST:
            state = TESTS_STATE;
            break;

        case TESTS_STATE:
            state = MOVE_TO_FIRST;
            break;
            
        case MOVE_TO_CONSTRUCTION:
            if (!secondIsBuilt) {
                movementStep++;
                if (movementStep > 1) {
                    state = BUILD_BLEACHER;
                    movementStep = 0;
                }
            }
            break;
            
        case MOVE_TO_SECOND:
            movementStep++;
            if (movementStep > 1) {
                state = GRAB_MATERIALS;
                movementStep = 0;
            }
            break;
    }
}