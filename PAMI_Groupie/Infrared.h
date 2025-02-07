#ifndef INFRARED_H
#define INFRARED_H

#include <SoftwareSerial.h>

const int LEFT_IR_SENSOR = 7;
const int RIGHT_IR_SENSOR = 8;

void initInfrared();
int returnDirection();
int returnDirection2();

#endif