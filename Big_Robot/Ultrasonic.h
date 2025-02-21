#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Number of sensors
#define NUM_ULTRASONIC 3  

// Define trig and echo pins for each sensor
const int trigPins[NUM_ULTRASONIC] = {9, 7, 5};
const int echoPins[NUM_ULTRASONIC] = {10, 8, 6};

void initUltrasonic();
float readDistance(int sensorIndex);

#endif
