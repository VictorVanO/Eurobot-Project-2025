#include "FSM.h"

FSM::FSM() :    state(INIT), startTime(0), obstacle_treshold(18), secondIsBuilt(false), armsFullyExtended(false),
                moveStartTime(0), moveDuration(0), isMovingBackward(false) {
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
    if (moveFunction == moveBackward) {
        isMovingBackward = true;
    } else {
        isMovingBackward = false;
    }
    // isMovingBackward = (moveFunction == moveBackward);

    moveFunction(speed);
    moveStartTime = millis();
    moveDuration = duration;

    nextState = next;
}


bool FSM::isObstacleDetected() {
    if (!isMovingBackward) {
        for (int i = 0; i < 3; i++) {
            float distance = readDistance(i);
            if (distance > 0 && distance <= obstacle_treshold) {
                return true;
            }
        }
    }
    
    if (isMovingBackward) {
        for (int i = 3; i < NUM_ULTRASONIC; i++) {
            float distance = readDistance(i);
            Serial.println(distance);
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

    if (moveStartTime > 0) {
        if (currentTime - moveStartTime >= moveDuration) {
            stopMotors();
            moveStartTime = 0;
            isMovingBackward = false;
            state = nextState;
            return;
        }
        
        // Change to Pause state if there is an obstacle during movement
        if (isObstacleDetected() && state != PAUSE && state) {
            stopMotors();
            moveStartTime = 0;
            isMovingBackward = false;
            previousState = state;
            state = PAUSE;
            pauseStartTime = currentTime;
            return;
        }
        return;
    }

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
            if (moveStartTime == 0) {
                Serial.println("State: MOVE TO FIRST");
                startTimedMovement(moveForward, 220, 3000, TESTS_STATE);
            }
            break;

        case TESTS_STATE:
            if (moveStartTime == 0) {
                Serial.println("State: TESTS");
                startTimedMovement(moveForward, 160, 3000, MOVE_TO_FIRST);
            }
            break;
        
        case GRAB_MATERIALS:
            Serial.println("State: Grab Materials");
            stopMotors();
            arms->grabMaterials();
            state = MOVE_TO_CONSTRUCTION;
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
            } else {
                state = GO_HOME;
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
                } else {
                    state = previousState;
                }
            }
            break;
        
        case AVOID_OBSTACLE:
            Serial.println("State: AVOID OBSTACLE");
            obstacle_treshold = 8;
            static int avoidStep = 0;

            switch (avoidStep) {
                case 0:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        Serial.println(isMovingBackward);
                        startTimedMovement(moveBackward, 160, 4000, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 1:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        Serial.println(isMovingBackward);
                        startTimedMovement(turnRight, 160, 600, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 2:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        startTimedMovement(moveForward, 220, 2000, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 3:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        startTimedMovement(turnLeft, 160, 1200, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 4:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        startTimedMovement(moveForward, 160, 2000, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 5:
                    if (moveStartTime == 0) {
                        Serial.println(avoidStep);
                        startTimedMovement(turnRight, 160, 600, previousState);
                        avoidStep = 0;
                        obstacle_treshold = 18;
                    }
                    break;
            }
            break;
    }
}