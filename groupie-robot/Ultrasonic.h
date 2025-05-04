#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

#define TRIG_PIN_1 9 
#define ECHO_PIN_1 10 
#define ECHO_PIN_2 7 
#define TRIG_PIN_2 8  

void initUltrasonic();
long getDistance(int sensor);

#endif
