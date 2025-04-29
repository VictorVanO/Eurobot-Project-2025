#include "FSM.h"

FSM::FSM() : state(INIT), startTime(0), obstacle_treshold(8), isYellow(false),
             isMovingBackward(false), zipperPulled(false), ultrasonicEnabled(true) {
    lcd = new LCD();
}

FSM::~FSM() {
    delete lcd;
}

void FSM::init() {
    pinMode(ZIPPER_PIN, INPUT_PULLUP);
    initMotors();
    initEncoders();
    initUltrasonic();
    initServomotors();
    lcd->init();
}

void FSM::run() {
    handleState();
}

bool FSM::isObstacleDetected() {
    if (!ultrasonicEnabled) return false;
    
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
    const unsigned long timeOutDelay = 92000;
    static bool goHomeWaitingForTimeout = false;
    static int goHomeStep = 0;
    static bool goHomeMotionStarted = false;
    static unsigned long pauseStartTime = 0;

    switch (state) {
        case INIT:
            Serial.println("State: INIT");
            lcd->printLine(0, "INIT");
            lcd->printLine(1, "Points: 0");

            if (digitalRead(ZIPPER_PIN) == LOW) {
                Serial.println("Tirette tirée ! Passage à DROP_BANNER");
                zipperPulled = true;
                if (startTime == 0) startTime = millis();
                state = DROP_BANNER;
                delay(100);
            } else {
                // Anti-spam delay
                delay(100);
            }

            // Stop the robot
            if (previousState == GO_HOME) {
                break;
            }
            break;
        
        case DROP_BANNER:
            Serial.println("State: DROP BANNER");
            lcd->printLine(0, "DROP BANNER");
            lcd->printLine(1, "Points: 0");
            state = GO_HOME;
            break;

        case GO_HOME:
            Serial.println("State: GO_HOME");
        
            // Start motion only once per step
            if (!goHomeMotionStarted) {
                switch (goHomeStep) {
                    case 0:
                        lcd->printLine(0, "Going Home...");
                        lcd->printLine(1, "Points: 0");
                        if (isYellow) {
                            openGripper(0);
                            ultrasonicEnabled = false;
                            startGoStraight(30);
                        } else {
                            openGripper(1);
                            startGoStraight(-30);
                        }
                        break;
                    case 1:
                        if (isYellow) {
                            openGripper(1);
                            ultrasonicEnabled = false;
                            startGoStraight(-30);
                        } else {
                            openGripper(0);
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
                            startGoStraight(112);
                        } else {
                            startGoStraight(11);
                        }
                        break;
                    case 4:
                        if (isYellow) {
                            startSmoothRotate(45,0);
                        } else {
                            startSmoothRotate(90, 0);
                        }
                        break;
                    case 5:
                        if (isYellow) {
                            if (!goHomeWaitingForTimeout) {
                                if (currentTime - startTime >= timeOutDelay) {
                                    startGoStraight(40);
                                    lcd->printLine(1, "Points: 10");
                                    goHomeWaitingForTimeout = true;
                                } else {
                                    return;
                                }
                            }
                        } else {
                            startGoStraight(55);
                        }
                        break;
                    case 6:
                        if (!isYellow) {
                            startSmoothRotate(90, 1);
                        }
                        break;
                    case 7:
                        if (!isYellow) {
                            startGoStraight(62);
                        }
                        break;
                    case 8:
                        if (!isYellow) {
                            if (!goHomeWaitingForTimeout) {
                                if (currentTime - startTime >= timeOutDelay) {
                                    startGoStraight(30);
                                    lcd->printLine(1, "Points: 10");
                                    goHomeWaitingForTimeout = true;
                                } else {
                                    return;
                                }
                            }
                        }
                        break;
                    case 9:
                        Serial.println("GO_HOME finished. Returning to INIT.");
                        goHomeStep = 0;
                        goHomeMotionStarted = false;
                        previousState = state;
                        state = INIT;
                        break;
                }
                goHomeMotionStarted = true;
            }
        
            updateMotion();
        
            if (isObstacleDetected()) {
                Serial.println("Obstacle detected during GO_HOME step. Pausing.");
                stopMotors();
                resetPIDVariables();
                previousState = GO_HOME;
                state = PAUSE;
                pauseStartTime = millis();
                return;
            }
        
            if (isMotionComplete()) {
                ultrasonicEnabled = true;
                goHomeStep++;
                goHomeMotionStarted = false;
            }
            break;        
        
        case PAUSE:
            Serial.println("State: PAUSE");
            stopMotors();
            if (currentTime - pauseStartTime >= 2000) {
                if (isObstacleDetected()) {
                    state = PAUSE;
                } else {
                    resumeMotion();
                    state = previousState;
                }
            }
            break;
    }
}