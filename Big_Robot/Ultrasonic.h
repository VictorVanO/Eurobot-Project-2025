#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Define the number of sensors
#define NUM_ULTRASONIC 3

const int trigPins[NUM_ULTRASONIC] = {9, 22, 26};
const int echoPins[NUM_ULTRASONIC] = {10, 24, 28};

void initUltrasonic();
float readDistance(int sensorIndex);

#endif
