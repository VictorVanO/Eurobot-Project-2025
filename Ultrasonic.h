#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

#define TRIG_PIN_1 9  // Pin TRIG du HC-SR04
#define ECHO_PIN_1 10 // Pin ECHO du HC-SR04
#define ECHO_PIN_2 7 // Pin ECHO du HC-SR04
#define TRIG_PIN_2 8  // Pin TRIG du HC-SR04


void initUltrasonic();
long getDistance(int sensor);

#endif