#include "FSM_Groupie.h"

FSM fsm;

void setup() {
    Serial.begin(9600);
    fsm.init();
}

void loop() {
    fsm.run();
    delay(100);
}
