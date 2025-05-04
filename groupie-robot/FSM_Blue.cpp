#include "FSM_Blue.h"
#include <Servo.h>

const int ledPin = 13;

// Obstacle avoidance step states
const int AVOID_NONE = 0;
const int AVOID_TURN_LEFT_DONE = 1;
const int AVOID_COMPLETED = 2;

// Authorize FSM to start
void FSM_Blue::authorizeStart() {
    startAuthorized = true;
    startTime = millis();
    globalTimer = millis();
}

// Constructor
FSM_Blue::FSM_Blue() {
    state = IDLE;
}

// Initialization function
void FSM_Blue::init() {
    Serial.begin(9600);
    initMotors();
    initUltrasonic();
    initEncoders();
    motorSpeed = 65;
    setMotorsSpeed(motorSpeed);
    state = IDLE;
    startTime = millis();
    globalTimer = millis();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

// Main FSM execution loop
void FSM_Blue::run() {
    handleState();
    updateEncoders();
}

// Handles FSM state logic
void FSM_Blue::handleState() {
    long distance1 = getDistance(1);
    long distance2 = getDistance(2);

    // Stop everything and enter PARTY_STATE after 20 seconds
    if (millis() - globalTimer >= 20000 && state != PARTY_STATE) {
        Serial.println("Time's up! Shutting down.");
        stopMotors();
        unsigned long start = millis();
        Servo servo;
        servo.attach(4);

        // Party loop: servo oscillation + LED blinking
        while (millis() - start < 850000) {
            stopMotors();
            for (int angle = 0; angle <= 90; angle++) {
                servo.write(angle);
                delay(10);
            }
            for (int angle = 90; angle >= 0; angle--) {
                servo.write(angle);
                delay(10);
            }
            digitalWrite(ledPin, HIGH);
            delay(250);
            digitalWrite(ledPin, LOW);
            delay(250);
        }
        state = PARTY_STATE;
    }

    switch (state) {
        case IDLE:
            stopMotors();
            if (millis() - startTime >= 5000) {
                state = FOLLOW_LINE_STATE;
                startTime = millis();
            }
            break;

        case FOLLOW_LINE_STATE:
            // Detect obstacle
            if (distance1 <= 9 || distance2 <= 9) {
                state = AWAIT_OBSTACLE_STATE;
            }

            // Temporary timed behavior
            if (millis() - startTime >= 11000) {
                while (millis() - startTime < 13000) {
                    turnLeft();
                    goForward();
                }
            }

            // Line-following direction handling
            switch (returnDirection()) {
                case 0:
                    goForward();
                    break;
                case 1:
                    turnLeft();
                    break;
                case 2:
                    turnRight();
                    break;
                case 3:
                    goForward();
                    break;
            }
            break;

        case AWAIT_OBSTACLE_STATE:
            stopMotors();

            distance1 = getDistance(1);
            distance2 = getDistance(2);

            if (distance1 >= 9 && distance2 >= 9) {
                state = FOLLOW_LINE_STATE;
            }

            obstacleAvoidance();
            break;

        case WAIT_CLEAR_STATE:
            stopMotors();
            long d1 = getDistance(1);
            long d2 = getDistance(2);

            if (d1 > 9 && d2 > 9) {
                if (avoidStep == AVOID_TURN_LEFT_DONE) {
                    goForward();
                    delay(600);
                    stopMotors();
                    delay(400);

                    resetCounts();
                    turnLeft();
                    while (getLeftCount() < ticksRotation) {
                        updateEncoders();
                    }
                    stopMotors();
                    delay(300);
                }

                avoidStep = AVOID_COMPLETED;
                state = FOLLOW_LINE_STATE;
            }
            break;

        case PARTY_STATE:
            Serial.println("Party time!");
            stopMotors();
            // party() logic already handled above
            break;
    }
}

// Handles obstacle avoidance routine
void FSM_Blue::obstacleAvoidance() {
    resetCounts();
    turnRight();

    const long targetTicks = 16;
    while (getRightCount() < targetTicks) {
        updateEncoders();
    }
    stopMotors();

    ticksRotation = getRightCount(); // Save number of ticks for left turn after
    delay(300);
    avoidStep = AVOID_TURN_LEFT_DONE;

    long d3 = getDistance(1);
    long d4 = getDistance(2);
    if (d3 < 9 || d4 < 9) {
        state = WAIT_CLEAR_STATE;
        return;
    }

    goForward();
    delay(600);
    stopMotors();
    delay(400);

    resetCounts();
    turnLeft();
    while (getLeftCount() < ticksRotation) {
        updateEncoders();
    }
    stopMotors();
    delay(300);
    avoidStep = AVOID_COMPLETED;
    state = FOLLOW_LINE_STATE;
}
