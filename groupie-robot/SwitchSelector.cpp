#include "SwitchSelector.h"

ModeSelector::ModeSelector(int pin) {
    switchPin = pin;
}

void ModeSelector::begin() {
    pinMode(switchPin, INPUT_PULLUP); // suppose une position reliée à GND et l’autre à 5V
}

bool ModeSelector::isPrimaryMode() {
    return digitalRead(switchPin) == HIGH; // HIGH = FSM actuelle, LOW = autre mode
}