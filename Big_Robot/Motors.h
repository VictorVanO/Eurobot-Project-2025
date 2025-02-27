#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// Define pin names
#define pwm_1 6
#define dir_1 7
#define pwm_2 4
#define dir_2 5

void initMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif