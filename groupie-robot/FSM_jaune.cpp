#include "FSM_jaune.h"
#include <Servo.h>

const int ledPin =13;
const int AVOID_NONE = 0;
const int AVOID_TURN_LEFT_DONE = 1;
const int AVOID_COMPLETED = 2;

void FSM_jaune::autoriserDemarrage() {
    demarrageAutorise = true;
    startTime = millis();
    globalTimer = millis();
}

// Constructeur
FSM_jaune::FSM_jaune() {
    state = IDLE;  // État initial
}

// Initialisation de la FSM
void FSM_jaune::init() {
    Serial.begin(9600);
    initMotors();  // Initialiser les moteurs
    initUltrasonic();
    initEncoders();
    //initServoLed();
    motorSpeed =65;
    setMotorsSpeed(motorSpeed);
    state = IDLE; 
    startTime = millis();
    globalTimer = millis();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

}

// Exécution de la FSM
void FSM_jaune::run() {
    handleState();
    updateEncoders();
}

// Gestion de l'état
void FSM_jaune::handleState() {

    long distance1 = getDistance(1); // Distance du capteur 1
    long distance2 = getDistance(2); // Distance du capteur 2
    // Serial.println(distance1);
    // Serial.println(distance2);


    
    // Serial.print("Ticks G : ");
    // Serial.print(getLeftCount());
    // Serial.print(" | D : ");
    // Serial.println(getRightCount());

    if (millis() - globalTimer >= 100000 && state != PARTY_STATE) {
        Serial.println("Temps écoulé ! Arrêt complet.");
        stopMotors();        
        unsigned long start = millis();
        Servo servo;
        servo.attach(4);
        while (millis() - start < 10000) {
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
            //  Serial.println("État : IDLE");
            stopMotors();
            if (millis() - startTime >= 85000){
                state = FOLLOW_LINE_STATE;
                startTime = millis();
            }
            break;

            case FOLLOW_LINE_STATE:

                if (distance1 <= 9 || distance2 <= 9) {
                    state = AWAIT_OBSTACLE_STATE;
                }
                if (millis() - startTime >= 13000){
                        while(millis() - startTime<15000){
                            turnRight();
                            goForward();
                        }
                    }
                
        
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
            // Serial.println("État : Avoid");

            stopMotors();

            distance1 = getDistance(1);
            distance2 = getDistance(2);

            if (distance1 >= 9 && distance2 >= 9) {  
                state = FOLLOW_LINE_STATE;
            }

            evitementObstacle();
            break;

        case WAIT_CLEAR_STATE:
            // Serial.println("État : clear");

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
                    turnRight();
                    while (getRightCount() < ticksRotation) {
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
            Serial.println("Party time !");
            stopMotors();
            break;
            
    }
}

void FSM_jaune::evitementObstacle() {
    // Serial.println("Evittement");
    resetCounts();
    turnLeft();

    const long ticksCible = 16; 
    while (getLeftCount() < ticksCible) {
        updateEncoders();
    }
    stopMotors();

    ticksRotation = getLeftCount() + 7;
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
    turnRight();

    while (getRightCount() < ticksRotation) {
        updateEncoders();
    }
    stopMotors();
    delay(300);
    avoidStep = AVOID_COMPLETED;
    state = FOLLOW_LINE_STATE;
}