#include "FSM_bleu_superstar.h"

void FSM_bleu::autoriserDemarrage() {
    demarrageAutorise = true;
    startTime = millis();     // Lancer les timers ici, proprement
    globalTimer = millis();
    obstacleStart = millis();
    obstacleTotalTime = 0;
}

// Constructeur
FSM_bleu::FSM_bleu() {
    state = IDLE;  // État initial
}

// Initialisation de la FSM
void FSM_bleu::init() {
    Serial.begin(9600);
    Serial.println("FSM initialisée");
    initMotors();  // Initialiser les moteurs
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

// Exécution de la FSM
void FSM_bleu::run() {
    handleState(); 
}

// Gestion de l'état
void FSM_bleu::handleState() {
    if (!demarrageAutorise) {
        Serial.println("En attente de la tirette...");
        stopMotors();
        return;
    }
    
    long distance1 = getDistance(1); // Distance du capteur 1
    long distance2 = getDistance(2); // Distance du capteur 2

    Serial.print("Distance mesurée : ");
    Serial.print(distance1);
    Serial.println(" cm");

    Serial.print("Distance mesurée : ");
    Serial.print(distance2);
    Serial.println(" cm");

    if (millis() - globalTimer >= 100000 && state != PARTY_STATE) {
        Serial.println("Temps écoulé ! Arrêt complet.");
        state = PARTY_STATE;
    }

    switch (state) {
        case IDLE:
            Serial.println("État : IDLE");
            stopMotors();
            if (millis() - startTime >= 85000){
              state = MOVE_FORWARD_STATE;
              startTime = millis();
            }
            break;

        case MOVE_FORWARD_STATE:
            if (millis() - startTime - obstacleTotalTime < 6000) {
                Serial.println("État : AVANCER");
                goForward();
            } 
            else if (millis() - startTime - obstacleTotalTime < 7500) {
                Serial.println("État : TOURNER À DROITE");
                setMotorsSpeed(motorSpeed1);
                turnLeft();
            } 
            else if (millis() - startTime - obstacleTotalTime <= 8000) {
                Serial.println("État : REPARTIR TOUT DROIT");
                goForward();
                setMotorsSpeed(motorSpeed2);
            }
            else {
                Serial.println("Fin de la séquence");
                state = PARTY_STATE;
            }
              
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
            Serial.println("Party time !");
            stopMotors();
            party();
            break;
    }
}
