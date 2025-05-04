#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Servo.h>

#define NUM_GRIPPERS 2

const int gripperPins[NUM_GRIPPERS] = {20, 21};

void initServomotors();
void openGripper(int index);
void closeGripper(int index);

#endif
