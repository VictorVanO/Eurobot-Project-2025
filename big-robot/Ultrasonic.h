#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Define the number of sensors
#define NUM_ULTRASONIC 3

const int trigPins[NUM_ULTRASONIC] = {22, 24, 26};
const int echoPins[NUM_ULTRASONIC] = {23, 25, 27};

void initUltrasonic();
float readDistance(int sensorIndex);

#endif
