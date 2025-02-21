#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Sensor 1
const int trigPin1 = 22;
const int echoPin1 = 24;

// Sensor 2
const int trigPin2 = 26;
const int echoPin2 = 28;

// Sensor 3
const int trigPin3 = 30;
const int echoPin3 = 32;

void initUltrasonic();
float readDistance(int trigPin, int echoPin);

#endif
