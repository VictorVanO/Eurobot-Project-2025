#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

const int PWM_1 = 6;
const int DIR_1 = 7;
const int PWM_2 = 4;
const int DIR_2 = 5;

void initMotors();
void setMotor(int dir, int pwmVal, int pwm, int pinDir);
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif