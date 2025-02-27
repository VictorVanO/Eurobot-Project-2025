#include "Ultrasonic.h"

void initUltrasonic() {
    pinMode(TRIG_PIN_1, OUTPUT);
    pinMode(ECHO_PIN_1, INPUT);
    pinMode(TRIG_PIN_2, OUTPUT);
    pinMode(ECHO_PIN_2, INPUT);
}

long getDistance(int sensor) {
    int trigPin, echoPin;
    
    if (sensor == 1) {
        trigPin = TRIG_PIN_1;
        echoPin = ECHO_PIN_1;
    } else {
        trigPin = TRIG_PIN_2;
        echoPin = ECHO_PIN_2;
    }
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2; // Conversion en cm
    return distance;
}
