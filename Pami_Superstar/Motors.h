#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>

extern Adafruit_MotorShield AFMS;
extern Adafruit_DCMotor *motor1;
extern Adafruit_DCMotor *motor2;

void initMotors();
void setMotorsSpeed(uint8_t speed);
void runMotors(uint8_t direction);
void goForward();
void turnLeft();
void turnRight();
void stopMotors();

#endif
