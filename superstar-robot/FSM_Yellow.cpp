#include "FSM_Yellow.h"

// Trigger the FSM start when the pull-pin is removed
void FSM_Yellow::authorizeStart() {
    startAuthorized = true;
    startTime = millis();    
    globalTimer = millis();
    obstacleStart = millis();
    obstacleTotalTime = 0;
}

// Constructor
FSM_Yellow::FSM_Yellow() {
    state = IDLE;
}

// Initialize all components and FSM state
void FSM_Yellow::init() {
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
}

// Main execution loop for the FSM
void FSM_Yellow::run() {
    handleState();
}

// Core FSM logic based on current state
void FSM_Yellow::handleState() {
    digitalWrite(ledPin, LOW);

    if (!startAuthorized) {
        Serial.println("Waiting for pull-pin...");
        stopMotors();
        return;
    }

    long distance1 = getDistance(1);
    long distance2 = getDistance(2);

    Serial.print("Distance 1: ");
    Serial.print(distance1);
    Serial.println(" cm");

    Serial.print("Distance 2: ");
    Serial.print(distance2);
    Serial.println(" cm");

    // Automatically stop after 100 seconds
    if (millis() - globalTimer >= 100000 && state != PARTY_STATE) {
        Serial.println("Time limit reached. Stopping...");
        state = PARTY_STATE;
    }

    switch (state) {
        case IDLE:
            Serial.println("State: IDLE");
            stopMotors();
            if (millis() - startTime >= 87000) {
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
                Serial.println("State: TURN RIGHT");
                setMotorsSpeed(motorSpeed1);
                turnRight();
            } 
            else if (millis() - startTime - obstacleTotalTime <= 8000) {
                Serial.println("State: STRAIGHT AGAIN");
                goForward();
                setMotorsSpeed(motorSpeed2);
            } 
            else {
                Serial.println("End of movement sequence");
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
