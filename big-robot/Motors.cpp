#include "Motors.h"
#include <Arduino.h>

void initMotors() {
    pinMode(PWM_1, OUTPUT);
    pinMode(DIR_1, OUTPUT);

    pinMode(PWM_2, OUTPUT);
    pinMode(DIR_2, OUTPUT);
}

void moveForward(int speed) {
    digitalWrite(DIR_1, 1);
    digitalWrite(DIR_2, 1);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void moveBackward(int speed) {
    digitalWrite(DIR_1, 0);
    digitalWrite(DIR_2, 0);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void turnLeft(int speed) {
    digitalWrite(DIR_1, 0);
    digitalWrite(DIR_2, 1);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void turnRight(int speed) {
    digitalWrite(DIR_1, 1);
    digitalWrite(DIR_2, 0);
    analogWrite(PWM_1, speed);
    analogWrite(PWM_2, speed);
}

void stopMotors() {
    analogWrite(PWM_1, 0);
    analogWrite(PWM_2, 0);
}