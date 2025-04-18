#include "FSM.h"

FSM::FSM() : state(INIT), startTime(0), obstacle_treshold(15), isYellow(true), armsFullyExtended(false),
             moveStartTime(0), moveDuration(0), isMovingBackward(false) {
    lcd = new LCD();
    // arms = new ServoArms();
}

FSM::~FSM() {
    delete lcd;
    // delete arms;
}

void FSM::init() {
    initMotors();
    initEncoders();
    initUltrasonic();
    lcd->init();
    // arms->init();
}

void FSM::run() {
    handleState();
}

void FSM::startTimedMovement(void (*moveFunction)(int), int speed, unsigned long duration, RobotState next) {
    // Determine if the robot is moving backward
    isMovingBackward = (moveFunction == moveBackward);

    // Start the movement
    moveFunction(speed);
    moveStartTime = millis();
    moveDuration = duration;
    nextState = next;
}

bool FSM::isObstacleDetected() {
    // Check front sensors when moving forward
    if (!isMovingBackward) {
        for (int i = FRONT_SENSORS_START; i < FRONT_SENSORS_START + NUM_FRONT_SENSORS; i++) {
            float distance = readDistance(i);
            Serial.print("Front sensor ");
            Serial.print(i);
            Serial.print(" distance: ");
            Serial.println(distance);
            if (distance > 0 && distance <= obstacle_treshold) {
                Serial.println("Front obstacle detected!");
                return true;
            }
        }
    }

    // Check back sensors when moving backward
    if (isMovingBackward) {
        for (int i = BACK_SENSORS_START; i < BACK_SENSORS_START + NUM_BACK_SENSORS; i++) {
            float distance = readDistance(i);
            Serial.print("Back sensor ");
            Serial.print(i);
            Serial.print(" distance: ");
            Serial.println(distance);
            if (distance > 0 && distance <= obstacle_treshold) {
                Serial.println("Back obstacle detected!");
                return true;
            }
        }
    }

    return false;
}

void FSM::handleState() {
    unsigned long currentTime = millis();
    static unsigned long pauseStartTime = 0;
    static int avoidStep = 0;  // Made static outside of the AVOID_OBSTACLE case

    // Check for obstacles while moving and handle accordingly
    if (moveStartTime > 0) {
        // Check for obstacle detection during movement
        if (isObstacleDetected()) {
            Serial.print("Obstacle detected during ");
            Serial.print(isMovingBackward ? "backward" : "forward");
            Serial.print(" movement in state: ");
            Serial.println(state);

            // Special handling for AVOID_OBSTACLE state
            if (state == AVOID_OBSTACLE) {
                Serial.print("Stopping during avoidance step: ");
                Serial.println(avoidStep);
                
                stopMotors();
                moveStartTime = 0;
                
                // For backward movement (step 0), skip to step 1
                if (avoidStep == 0 && isMovingBackward) {
                    Serial.println("Skipping backward step due to obstacle");
                    avoidStep = 1;
                } 
                // For forward movement (step 2 or 4), try a different direction
                else if ((avoidStep == 2 || avoidStep == 4) && !isMovingBackward) {
                    Serial.println("Trying alternative avoidance path");
                    // If we hit an obstacle during forward movement, turn more
                    avoidStep = 1;  // Go back to turning step
                }
                // For turning movements, just proceed to next step
                else {
                    avoidStep++;
                    if (avoidStep > 5) {
                        avoidStep = 0;
                        state = previousState;
                    }
                }
                
                isMovingBackward = false;
                return;
            }
            // Regular obstacle handling for other states
            else if (state != PAUSE) {
                Serial.println("Regular obstacle handling");
                stopMotors();
                moveStartTime = 0;
                isMovingBackward = false;
                previousState = state;
                state = PAUSE;
                pauseStartTime = currentTime;
                return;
            }
        }
        
        // Stop motors when movement is finished
        if (currentTime - moveStartTime >= moveDuration) {
            stopMotors();
            moveStartTime = 0;
            isMovingBackward = false;
            
            if (state == AVOID_OBSTACLE) {
                // Stay in AVOID_OBSTACLE state but move to next step
                // The next state will be set when all avoidance steps are complete
            } else {
                state = nextState;
            }
            return;
        }
        
        return;
    }

    // Main state machine logic
    switch (state) {
        case INIT:
            Serial.println("State: INIT");
            if (startTime == 0) startTime = millis();
            state = DROP_BANNER;
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
                // startTimedMovement(moveForward, 160, 3000, MOVE_TO_FIRST);
                break;
            }
            break;
        
        case DROP_BANNER:
            Serial.println("State: DROP BANNER");
            // Serial.println("State: Drop Banner");
            // arms->openHands(2);
            state = GO_HOME;
            break;
        
        case GO_HOME:
            Serial.println("State: GO_HOME");
        
            static int goHomeStep = 0;
        
            if (getCurrentMotionType() == MOTION_NONE) {
                switch (goHomeStep) {
                    case 0:
                        if (isYellow) {
                            startGoStraight(30);
                        } else {
                            startGoStraight(-30);
                        }
                        break;
                    case 1:
                        if (isYellow) {
                            startGoStraight(-20);
                        } else {
                            startGoStraight(20);
                        }
                        break;
                    case 2:
                        if (isYellow) {
                            startSmoothRotate(45, 0);
                        } else {
                            startSmoothRotate(90, 0);
                        }
                        break;
                    case 3:
                        if (isYellow) {
                            startGoStraight(8);
                        } else {
                            startGoStraight(8);
                        }
                        break;
                    case 4:
                        if (isYellow) {
                            startRotate(45);
                        } else {
                            startSmoothRotate(90, 0);
                        }
                        break;
                    case 5:
                        if (isYellow) {
                            startGoStraight(55);
                        } else {
                            startGoStraight(58);
                        }
                        break;
                    case 6:
                        if (!isYellow) {
                            startSmoothRotate(90, 1);
                        }
                        break;
                    case 7:
                        if (!isYellow) {
                            startGoStraight(70);
                            delay(5000);
                            startGoStraight(87);
                            delay(60000);
                            startGoStraight(20);
                        }
                        break;
                    case 8:
                        Serial.println("GO_HOME finished. Returning to INIT.");
                        goHomeStep = 0;
                        state = TESTS_STATE;  // Ou autre état suivant
                        break;
                }

                if (goHomeStep < 8) {
                    goHomeStep++;
                }
            } else {
                updateMotion();

                if (isObstacleDetected()) {
                    Serial.println("Obstacle detected during GO_HOME motion.");
                    stopMotors();
                    resetPIDVariables();
                    previousState = GO_HOME;
                    state = PAUSE;
                    pauseStartTime = millis();
                    return;
                }
            }
            break;
        
        case PAUSE:
            Serial.println("State: PAUSE");
            stopMotors();
            if (currentTime - pauseStartTime >= 3000) {
                if (isObstacleDetected()) {
                    // state = AVOID_OBSTACLE;
                    // avoidStep = 0;  // Reset avoidance step counter
                    state = PAUSE;
                } else {
                    state = previousState;
                }
            }
            break;
        
        case AVOID_OBSTACLE:
            Serial.println("State: AVOID OBSTACLE");
            Serial.print("Avoid step: ");
            Serial.println(avoidStep);

            switch (avoidStep) {
                case 0:
                    if (moveStartTime == 0) {
                        Serial.println("Starting backward movement for obstacle avoidance");
                        startTimedMovement(moveBackward, 160, 700, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 1:
                    // If we reach this point, it means the backward movement completed without
                    // obstacle detection or the obstacle detection was handled in the movement check
                    if (moveStartTime == 0) {
                        Serial.println("Starting right turn for obstacle avoidance");
                        startTimedMovement(turnRight, 160, 700, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 2:
                    if (moveStartTime == 0) {
                        Serial.println("Starting forward movement for obstacle avoidance");
                        startTimedMovement(moveForward, 220, 4000, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 3:
                    if (moveStartTime == 0) {
                        Serial.println("Starting left turn for obstacle avoidance");
                        startTimedMovement(turnLeft, 160, 1400, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 4:
                    if (moveStartTime == 0) {
                        Serial.println("Starting second forward movement for obstacle avoidance");
                        startTimedMovement(moveForward, 160, 4000, AVOID_OBSTACLE);
                        avoidStep++;
                    }
                    break;
                case 5:
                    if (moveStartTime == 0) {
                        Serial.println("Starting final right turn to resume previous state");
                        startTimedMovement(turnRight, 160, 600, previousState);
                        avoidStep = 0;  // Reset for next time
                    }
                    break;
            }
            break;
    }
}