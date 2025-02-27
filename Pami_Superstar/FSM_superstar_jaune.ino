#include "FSM_superstar.h"

FSM fsm;  // Création de l'instance de la FSM

void setup() {
    fsm.init();   // Initialiser la FSM et les moteurs
}

void loop() {
    fsm.run();    // Exécuter la FSM en continu
    delay(100);
}
