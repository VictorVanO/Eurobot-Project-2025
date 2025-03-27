#include "ButtonUrgence.h"

bool emergencyActive = false; 
void initEmergencyButton() {
    pinMode(emergencyButton, INPUT_PULLUP); 
}

bool isEmergencyActive() {
    return emergencyActive; 
}

void updateEmergencyState() {
    if (digitalRead(emergencyButton) == HIGH) { 
        emergencyActive = true;  
    }
}