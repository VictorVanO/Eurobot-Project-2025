#ifndef BUTTON_URGENCE_H
#define BUTTON_URGENCE_H

#include <Arduino.h>

const int emergencyButton = 13; 

void initEmergencyButton(); 
bool isEmergencyActive();   
void updateEmergencyState();
#endif