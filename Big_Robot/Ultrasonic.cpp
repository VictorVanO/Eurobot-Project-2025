#include "Ultrasonic.h"
#include <Arduino.h>

// Initialize all ultrasonic sensors
void initUltrasonic() {
    for (int i = 0; i < NUM_ULTRASONIC; i++) {
        pinMode(trigPins[i], OUTPUT);
        pinMode(echoPins[i], INPUT);
    }
}

// Read distance from a specific sensor
float readDistance(int sensorIndex) {
    if (sensorIndex < 0 || sensorIndex >= NUM_ULTRASONIC) return -1;  // Invalid sensor index

    float duration, distance;

    digitalWrite(trigPins[sensorIndex], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[sensorIndex], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[sensorIndex], LOW);

    duration = pulseIn(echoPins[sensorIndex], HIGH);
    distance = (duration * 0.0343) / 2;  // Convert to cm

    return distance;
}
