#include "FSM_Groupie.h"

FSM fsm;
const int tirettePin = 11;
Tirette tirette(tirettePin);  
bool demarrageAutorise = false;

void setup() {
    Serial.begin(9600);
    tirette.begin();  // configure la pin
    fsm.init();
}

void loop() {
    if (!demarrageAutorise) {
        if (!tirette.estActivee()) {
            Serial.println("Tirette retirée ! Démarrage autorisé.");
            demarrageAutorise = true;
        } else {
            Serial.println("En attente de la tirette...");
            delay(200);
            return;
        }
    }

    fsm.run();
    delay(100);
}
