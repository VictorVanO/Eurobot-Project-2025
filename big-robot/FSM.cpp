#include "FSM.h"

FSM::FSM() :    state(INIT), startTime(0), obstacle_treshold(18), secondIsBuilt(false), armsFullyExtended(false),
                moveStartTime(0), moveDuration(0), startAvoidance(0), isMovingBackward(false) {
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

void FSM::startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration, RobotState next) {
    isMovingBackward = (moveFunction == moveBackward);

    moveFunction(speed);
    moveStartTime = millis();
    moveDuration = duration;

    nextState = next;
}


bool FSM::isObstacleDetected() {
    for (int i = 0; i < 3; i++) {
        float distance = readDistance(i);
        if (distance > 0 && distance <= obstacle_treshold) {
            return true;
        }
    }
    
    if (isMovingBackward) {
        for (int i = 3; i < NUM_ULTRASONIC; i++) {
            float distance = readDistance(i);
            if (distance > 0 && distance <= obstacle_treshold) {
                return true;
            }
        }
    }
    return false;
}

void FSM::handleState() {
    unsigned long currentTime = millis();
    static unsigned long pauseStartTime = 0;
    const unsigned long backwardAvoidanceDuration = 600;
    const unsigned long turnDuration = 600;

    if (moveStartTime > 0) {
        // Check if movement time is complete
        if (currentTime - moveStartTime >= moveDuration) {
            stopMotors();
            moveStartTime = 0;
            isMovingBackward = false;
            state = nextState;
            return;
        }
        
        // Obstacle check during movement
        if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
            stopMotors();
            moveStartTime = 0;
            isMovingBackward = false;
            previousState = state;
            state = PAUSE;
            pauseStartTime = currentTime;
            Serial.println("Obstacle detected during movement! Switching to PAUSE state.");
            return;
        }
        return;
    }

    // Regular obstacle check when not moving
    // if (isObstacleDetected() && state != PAUSE && state != AVOID_OBSTACLE) {
    //     previousState = state;
    //     state = PAUSE;
    //     pauseStartTime = currentTime;
    //     Serial.println("Obstacle detected! Switching to PAUSE state.");
    //     return;
    // }

    switch (state) {
        case INIT:
            Serial.println("State: INIT");
            if (startTime == 0) startTime = millis();
            state = MOVE_TO_FIRST;
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
            startTimedMovement(moveForward, 220, 5000, TESTS_STATE);
            break;

        case TESTS_STATE:
            Serial.println("State: TESTS");
            arms->retractArms();
            startTimedMovement(moveForward, 160, 2500, MOVE_TO_FIRST);
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
                Serial.println("Moving to construction site");
                startTimedMovement(turnRight, 185, 120, BUILD_BLEACHER);
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
            startTimedMovement(turnLeft, 185, 100, GRAB_MATERIALS);
            break;
            
        case GO_HOME:
            startTimedMovement(moveForward, 185, 1000, INIT);
            break;

        case PAUSE:
            Serial.println("State: PAUSE");
            stopMotors();
            if (currentTime - pauseStartTime >= 2000) {
                if (isObstacleDetected()) {
                    state = AVOID_OBSTACLE;
                    startAvoidance = 0;
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
                
                bool backObstacle = false;
                for (int i = 3; i < NUM_ULTRASONIC; i++) {
                    float distance = readDistance(i);
                    if (distance > 0 && distance <= obstacle_treshold) {
                        backObstacle = true;
                        break;
                    }
                }
                
                if (backObstacle) {
                    stopMotors();
                    startAvoidance = millis() - backwardAvoidanceDuration;
                } else {
                    isMovingBackward = true;
                    moveBackward(220);
                }
            } else if (elapsedTime < backwardAvoidanceDuration + turnDuration) {
                isMovingBackward = false;
                turnRight(220);
            } else {
                stopMotors();
                isMovingBackward = false;
                startAvoidance = 0;
                state = previousState;
            }
            break;
    }
}