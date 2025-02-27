#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// Define pin names
const int PWM_1 = 6;
const int DIR_1 = 7;
const int PWM_2 = 4;
const int DIR_2 = 5;

// #define PWM_1 6
// #define DIR_1 7
// #define PWM_2 4
// #define DIR_2 5

void initMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif