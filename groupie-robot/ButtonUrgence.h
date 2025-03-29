#ifndef BUTTON_URGENCE_H
#define BUTTON_URGENCE_H

#include <Arduino.h>

const int emergencyButton = 4; 

void initEmergencyButton(); 
bool isEmergencyActive();   
void updateEmergencyState();
#endif