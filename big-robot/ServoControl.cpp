#include "ServoControl.h"
#include <Arduino.h>

Servo grippers[NUM_GRIPPERS];

void initServomotors() {
    for (int i = 0; i < NUM_GRIPPERS; i++) {
        grippers[i].attach(gripperPins[i]);
    }
}

void openGripper(int index) {
    if (index < 0 || index >= NUM_GRIPPERS) return;

    for (int angle = 0; angle <= 180; angle++) {
        grippers[index].write(angle);
        delay(15);
    }
}

void closeGripper(int index) {
    if (index < 0 || index >= NUM_GRIPPERS) return;

    for (int angle = 180; angle >= 0; angle--) {
        grippers[index].write(angle);
        delay(15);
    }
}
