#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Define the number of sensors
#define NUM_ULTRASONIC 6

const int trigPins[NUM_ULTRASONIC] = {22, 24, 26, 28, 30, 32};
const int echoPins[NUM_ULTRASONIC] = {23, 25, 27, 29, 31, 33};

void initUltrasonic();
float readDistance(int sensorIndex);

#endif
