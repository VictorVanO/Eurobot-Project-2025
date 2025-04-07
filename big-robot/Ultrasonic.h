#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define NUM_ULTRASONIC 6
#define NUM_FRONT_SENSORS 3
#define NUM_BACK_SENSORS 3

// Sensor indices
#define FRONT_SENSORS_START 0
#define BACK_SENSORS_START 3

const int trigPins[NUM_ULTRASONIC] = {22, 24, 26, 28, 30, 32};
const int echoPins[NUM_ULTRASONIC] = {23, 25, 27, 29, 31, 33};

void initUltrasonic();
float readDistance(int sensorIndex);
bool isFrontObstacleDetected(float threshold);
bool isBackObstacleDetected(float threshold);

#endif
