#include "FSM_Blue.h"

// Authorize the FSM to start (pull-pin removed)
void FSM_Blue::authorizeStart() {
    startAuthorized = true;
    startTime = millis();
    globalTimer = millis();
    obstacleStart = millis();
    obstacleTotalTime = 0;
}

// Constructor
FSM_Blue::FSM_Blue() {
    state = IDLE;
}

// Initialize robot components and FSM variables
void FSM_Blue::init() {
    Serial.begin(9600);
    Serial.println("FSM initialized");

    initMotors();
    initUltrasonic();
    initServoLed();

    motorSpeed = 90;
    motorSpeed1 = 55;
    motorSpeed2 = 50;

    setMotorsSpeed(motorSpeed);
    state = IDLE;

    startTime = millis();
    globalTimer = millis();
    obstacleStart = millis();
    obstacleTotalTime = 0;
}

// Main FSM execution loop
void FSM_Blue::run() {
    handleState();
}

// FSM logic depending on current state
void FSM_Blue::handleState() {
    if (!startAuthorized) {
        Serial.println("Waiting for pull-pin...");
        stopMotors();
        return;
    }

    long distance1 = getDistance(1);
    long distance2 = getDistance(2);

    Serial.print("Measured distance 1: ");
    Serial.print(distance1);
    Serial.println(" cm");

    Serial.print("Measured distance 2: ");
    Serial.print(distance2);
    Serial.println(" cm");

    // Safety timeout: stop after 100 seconds
    if (millis() - globalTimer >= 100000 && state != PARTY_STATE) {
        Serial.println("Time limit reached. Stopping...");
        state = PARTY_STATE;
    }

    switch (state) {
        case IDLE:
            Serial.println("State: IDLE");
            stopMotors();
            if (millis() - startTime >= 5000) {
                state = MOVE_FORWARD_STATE;
                startTime = millis();
            }
            break;

        case MOVE_FORWARD_STATE:
            if (millis() - startTime - obstacleTotalTime < 6000) {
                Serial.println("State: MOVE FORWARD");
                goForward();
            }
            else if (millis() - startTime - obstacleTotalTime < 7500) {
                Serial.println("State: TURN LEFT");
                setMotorsSpeed(motorSpeed1);
                turnLeft();
            }
            else if (millis() - startTime - obstacleTotalTime <= 8000) {
                Serial.println("State: STRAIGHT AGAIN");
                goForward();
                setMotorsSpeed(motorSpeed2);
            }
            else {
                Serial.println("End of sequence");
                state = PARTY_STATE;
            }

            // Check for obstacle
            if (distance1 < 7 || distance2 < 7) {
                obstacleStart = millis();
                state = AWAIT_OBSTACLE_STATE;
            }
            break;

        case AWAIT_OBSTACLE_STATE:
            stopMotors();
            delay(500);
            if (distance1 >= 7 && distance2 >= 7) {
                obstacleTotalTime += (millis() - obstacleStart);
                state = MOVE_FORWARD_STATE;
            }
            break;

        case PARTY_STATE:
            Serial.println("Party time!");
            stopMotors();
            party();
            break;
    }
}
