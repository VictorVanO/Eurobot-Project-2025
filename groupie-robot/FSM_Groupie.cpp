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
    //initServoLed();
    initEmergencyButton();
    motorSpeed =65;
    setMotorsSpeed(motorSpeed);
    state = IDLE; 
    startTime = millis();
    globalTimer = millis();
}

// Exécution de la FSM
void FSM::run() {
    updateEmergencyState();
    handleState(); 
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
    Serial.println(distance1);
    Serial.println(distance2);


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
            
          
                if (distance1 <= 25 || distance2 <= 25) {
                    state = AWAIT_OBSTACLE_STATE;
                }
                break;

        case AWAIT_OBSTACLE_STATE:
            stopMotors();
            delay(500);
            evitementObstacle();

            distance1 = getDistance(1);
            distance2 = getDistance(2);

            if (distance1 >= 25 || distance2 >= 25) {  
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
    
    turnLeft();
    delay(650);
    goForward();
    delay(400);
    turnRight();
    delay(650);
    stopMotors();
    delay(1000);
  }