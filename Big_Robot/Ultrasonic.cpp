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
