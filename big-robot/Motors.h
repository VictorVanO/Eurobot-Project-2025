#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// Define pin names
const int PWM_1 = 6;
const int DIR_1 = 7;
const int PWM_2 = 4;
const int DIR_2 = 5;

// Ou ces pins du code d'Edin ?
// #define PWM_1 7
// #define DIR_1 6
// #define PWM_2 5
// #define DIR_2 4

void initMotors();
void setMotor(int dir, int pwmVal, int pwm, int pinDir); // For PID
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif