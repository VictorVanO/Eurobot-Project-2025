#ifndef INFRARED_H
#define INFRARED_H

#include <SoftwareSerial.h>

const int LEFT_IR_SENSOR = 6;
const int RIGHT_IR_SENSOR = 5;

void initInfrared();
int returnDirection();

#endif