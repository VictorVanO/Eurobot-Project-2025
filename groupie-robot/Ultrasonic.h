#ifndef ULTRASONIC_H
#define ULTRASONIC_H

const int trigPin = 9;
const int echoPin = 10;

void initUltrasonic();
float readDistance();

#endif