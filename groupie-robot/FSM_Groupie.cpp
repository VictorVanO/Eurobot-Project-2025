#include "FSM_Groupie.h"

// Constructeur
FSM::FSM() {
    state = IDLE;  // État initial
}

// Initialisation de la FSM
void FSM::init() {
    Serial.begin(9600);
    initMotors();  // Initialiser les moteurs
    initUltrasonic();
    initEncoders();
    //initServoLed();
    initEmergencyButton();
    motorSpeed =50;
    setMotorsSpeed(motorSpeed);
    state = IDLE; 
    startTime = millis();
    globalTimer = millis();
}

// Exécution de la FSM
void FSM::run() {
    updateEmergencyState();
    handleState();
    updateEncoders();
}

// Gestion de l'état
void FSM::handleState() {
    if (isEmergencyActive()) {
        Serial.println("Arrêt d'urgence activé !");
        stopMotors();
        while (1);  // Boucle infinie pour stopper définitivement le robot
    }
    
    long distance1 = getDistance(1); // Distance du capteur 1
    long distance2 = getDistance(2); // Distance du capteur 2
    
    Serial.print("Ticks G : ");
    Serial.print(getLeftCount());
    Serial.print(" | D : ");
    Serial.println(getRightCount());

    if (millis() - globalTimer >= 20000 && state != PARTY_STATE) {
        Serial.println("Temps écoulé ! Arrêt complet.");
        state = PARTY_STATE;
    }
                
    switch (state) {
        case IDLE:
            Serial.println("État : IDLE");
            stopMotors();
            if (millis() - startTime >= 5000){
                state = FOLLOW_LINE_STATE;
                startTime = millis();
            }
            break;

            case FOLLOW_LINE_STATE:
                if (distance1 <= 18 || distance2 <= 18) {
                    state = AWAIT_OBSTACLE_STATE;
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
            stopMotors();
            evitementObstacle();

            distance1 = getDistance(1);
            distance2 = getDistance(2);

            if (distance1 >= 18 && distance2 >= 18) {  
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

void FSM::evitementObstacle() {
    resetCounts();
    turnLeft();

    const long ticksCible = 16; 
    while (getLeftCount() < ticksCible) {
        updateEncoders();
    }
    stopMotors();

    long ticksRotation = getLeftCount();
    delay(300);

    long d1 = getDistance(1);
    long d2 = getDistance(2);
    if (d1 < 9 || d2 < 9) {
        state = PARTY_STATE;  
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
}

