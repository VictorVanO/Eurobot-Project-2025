#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// Define pin names
#define PWM_2 4
#define PWM_1 7
#define DIR_2 5
#define DIR_1 6

// Function prototypes
void initMotors();
void sweepMotor();

#endif