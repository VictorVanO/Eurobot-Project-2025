#include "FSM_jaune_superstar.h"  
#include "FSM_bleu_superstar.h"       

const int modeSwitchPin = 3;
ModeSelector selector(modeSwitchPin);


FSM_jaune fsmJaune;
FSM_bleu fsmBleu;

bool useBlue = false;

void setup() {
    Serial.begin(9600);
    selector.begin();
    if (selector.isPrimaryMode()) {
        Serial.println("FSM_bleu");
        useBlue = true;
        fsmBleu.init(); 
    } else {
        Serial.println("FSM_jaune");
        useBlue = false;
        fsmJaune.init();  
    }
}

void loop() {
    if (useBlue) {
        fsmBleu.run();
    } else {
        fsmJaune.run();
    }
    delay(100);
}
