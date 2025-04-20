#include "FSM_jaune.h"  
#include "FSM_bleu.h"       

const int modeSwitchPin = 12;
ModeSelector selector(modeSwitchPin);

const int tirettePin = 11;
Tirette tirette(tirettePin); 

FSM_jaune fsmJaune;
FSM_bleu fsmBleu;

bool demarrageAutorise = false;
bool useBlue = false;

void setup() {
    Serial.begin(9600);
    selector.begin();
    tirette.begin();
    if (selector.isPrimaryMode()) {
        Serial.println("FSM_bleu");
        useBlue = true;
        fsmJaune.init(); 
    } else {
        Serial.println("FSM_jaune");
        useBlue = false;
        fsmJaune.init();  
    }
}

void loop() {
    if (!demarrageAutorise) {
        if (tirette.estActivee()) {
            Serial.println("Tirette activée, démarrage autorisé !");
            demarrageAutorise = true;
            if (useBlue) {
                fsmBleu.autoriserDemarrage();  
            } else {
                fsmJaune.autoriserDemarrage(); 
            }

        } else {
            Serial.println("En attente de la tirette...");
            delay(200);
            return;
        }
    }

    // FSM active uniquement après la tirette
    if (useBlue) {
        fsmBleu.run();
    } else {
        fsmJaune.run();
    }

    delay(100);
}