#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>

#define LEFT_ENCODER_PIN 3
#define RIGHT_ENCODER_PIN 2

void initEncoders();
void updateEncoders(); 
long getLeftCount();
long getRightCount();
void resetCounts();

#endif
