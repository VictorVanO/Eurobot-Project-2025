#include "Ultrasonic.h"
#include <Arduino.h>

void initUltrasonic() {
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        pinMode(trigPins[i], OUTPUT);
        pinMode(echoPins[i], INPUT);
    }
}

float readDistance(int sensorIndex) {
    float duration, distance;

    int trigPin = trigPins[sensorIndex];
    int echoPin = echoPins[sensorIndex];

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;

    return distance;
}


bool isFrontObstacleDetected(float threshold) {
    for (int i = FRONT_SENSORS_START; i < FRONT_SENSORS_START + NUM_FRONT_SENSORS; i++) {
        float distance = readDistance(i);
        if (distance > 0 && distance <= threshold) {
            return true;
        }
    }
    return false;
}

bool isBackObstacleDetected(float threshold) {
    for (int i = BACK_SENSORS_START; i < BACK_SENSORS_START + NUM_BACK_SENSORS; i++) {
        float distance = readDistance(i);
        if (distance > 0 && distance <= threshold) {
            return true;
        }
    }
    return false;
}