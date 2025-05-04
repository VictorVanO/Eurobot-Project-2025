#ifdef UNIT_TEST
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif

#include "FSM.h"
#include "ServoEasing.hpp"

FSM fsm;

void setup() {
    Serial.begin(9600);
    fsm.init();
}

void loop() {
    fsm.run();
    delay(100);
}
