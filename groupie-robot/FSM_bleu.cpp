#include "FSM_bleu.h"
#include <Servo.h>

const int ledPin =13;
const int AVOID_NONE = 0;
const int AVOID_TURN_LEFT_DONE = 1;
const int AVOID_COMPLETED = 2;

void FSM_bleu::autoriserDemarrage() {
    demarrageAutorise = true;
    startTime = millis();
    globalTimer = millis();
}

// Constructeur
FSM_bleu::FSM_bleu() {
    state = IDLE;  // État initial
}

// Initialisation de la FSM
void FSM_bleu::init() {
    Serial.begin(9600);
    initMotors();  // Initialiser les moteurs
    initUltrasonic();
    initEncoders();
    motorSpeed =65;
    setMotorsSpeed(motorSpeed);
    state = IDLE; 
    startTime = millis();
    globalTimer = millis();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

// Exécution de la FSM
void FSM_bleu::run() {
    handleState();
    updateEncoders();
}

// Gestion de l'état
void FSM_bleu::handleState() {
    
    
    long distance1 = getDistance(1); // Distance du capteur 1
    long distance2 = getDistance(2); // Distance du capteur 2
    // Serial.println(distance1);
    // Serial.println(distance2);


    
    // Serial.print("Ticks G : ");
    // Serial.print(getLeftCount());
    // Serial.print(" | D : ");
    // Serial.println(getRightCount());

    if (millis() - globalTimer >= 20000 && state != PARTY_STATE) {
        Serial.println("Temps écoulé ! Arrêt complet.");
        stopMotors();        
        unsigned long start = millis();
        Servo servo;
        servo.attach(4);
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
            //  Serial.println("État : IDLE");
            stopMotors();
            if (millis() - startTime >= 5000){
                state = FOLLOW_LINE_STATE;
                startTime = millis();
            }
            break;

            case FOLLOW_LINE_STATE:
                
                if (distance1 <= 9 || distance2 <= 9) {
                    state = AWAIT_OBSTACLE_STATE;
                }
                if (millis() - startTime >= 11000){ 
                    while(millis() - startTime<13000){
                            turnLeft();
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
                    turnLeft();  // 🔄 Inversé par rapport à jaune
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
            Serial.println("Party time !");
            stopMotors();
            //party();
            break;
    }
}

void FSM_bleu::evitementObstacle() {
    resetCounts();
    turnRight();

    const long ticksCible = 16;
    while (getRightCount() < ticksCible) {
        updateEncoders();
    }
    stopMotors();

    ticksRotation = getRightCount();
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
