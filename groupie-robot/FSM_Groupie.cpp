#include "FSM_Groupie.h"

FSM::FSM() : state(IDLE), startTime(0) {}

void FSM::init() {
    initMotors();
    initUltrasonic();
    initBluetooth();
    initInfrared();
    initServoLed();
}

void FSM::run() {
    handleState();
}

void FSM::handleState() {
    float distance = readDistance();
    const unsigned long timeoutDuration = 5000;  //!!!FINAL : 75 000
    const unsigned long timeoutParty = 20000;  //!!!FINAL : 100 000
    unsigned long currentTime = millis();
    
    switch (state) {
        case IDLE:
            if (startTime == 0){
              startTime = millis();
            }
            Serial.println("Robot state: Idle");
            Serial.print("Waiting 5 seconds before starting.");
            sendMessage("Robot state: Idle");
            // After 5 seconds, the robot starts if there is no obstacle.
            if (currentTime - startTime >= timeoutDuration) {
                if (distance > 0 && distance <= 5) {
                    state = OBSTACLE_WAIT;
                } else {
                    state = FOLLOW_LINE;
                }
            } else {
                state = IDLE;
            }
            break;

        case FOLLOW_LINE:
            sendMessage("Robot state: Follow Line");
            Serial.println(distance);
            setMotorsSpeed(70);
            switch (returnDirection()) {
                case 0:
                    Serial.println("Robot goes forward and follow white lines.");
                    goForward();
                    break;
                case 1:
                    Serial.println("Robot turn left.");
                    // stopMotors();
                    // delay(100);
                    turnLeft();
                    delay(50);
                    stopMotors();
                    break;
                case 2:
                    Serial.println("Robot turn right.");
                    // stopMotors();
                    // delay(100);
                    turnRight();
                    delay(50);
                    stopMotors();
                    break;
                case 3:
                    Serial.println("Robot stop because no white lines.");
                    stopMotors();
                    break;
                default:
                    break;
            }
            if (distance > 0 && distance <= 5) {
                state = OBSTACLE_WAIT;
            }
            break;

        case OBSTACLE_WAIT:
            Serial.println("Obstacle Detected : Robot stopping");
            sendMessage("Robot state: Obstacle Wait");
            Serial.println(distance);
            stopMotors();
            if (distance > 5) {
                state = FOLLOW_LINE;
                break;
            }
            else {
                state = OBSTACLE_WAIT;
            }
            break;
        
        case PAUSE:
            Serial.println("Robot state: Pause");
            sendMessage("Robot state: Pause");
            stopMotors();
            if (currentTime - startTime >= timeoutParty) {
                state = PARTY;
                break;
            }else {
                state = PAUSE;
            }
            if (distance > 0 && distance < 10) {
                state = OBSTACLE_WAIT;
            }
            break;
            
        case PARTY:
            sendMessage("Robot state: Party");
            Serial.println("Party time ! ");
            stopMotors();
            party();
    }
}