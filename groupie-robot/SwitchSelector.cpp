#include "SwitchSelector.h"

ModeSelector::ModeSelector(int pin) {
    switchPin = pin;
}

void ModeSelector::begin() {
    pinMode(switchPin, INPUT_PULLUP); 
}

bool ModeSelector::isPrimaryMode() {
    return digitalRead(switchPin) == HIGH;
}
