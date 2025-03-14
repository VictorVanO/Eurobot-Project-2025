#ifndef SERVOLED_H
#define SERVOLED_H

const int ledPin = 5;
const int resetButtonPin = 2; // Bouton de reset
const int startAngle = 0;
const int endAngle = 90;
const int stepDelay = 15;

void initServoLed();
void party();
void angle();


#endif