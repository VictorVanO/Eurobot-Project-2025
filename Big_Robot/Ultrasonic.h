#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Number of sensors
#define NUM_ULTRASONIC 3  

// Define trig and echo pins for each sensor
const int trigPins[NUM_ULTRASONIC] = {22, 26, 30};
const int echoPins[NUM_ULTRASONIC] = {24, 28, 32};

void initUltrasonic();
float readDistance(int sensorIndex);

#endif
