#include "Motors.h"

void initMotors() {
    pinMode(PWM_2, OUTPUT);
    pinMode(PWM_1, OUTPUT);
    pinMode(DIR_1, OUTPUT);
    pinMode(DIR_2, OUTPUT);
}

void sweepMotor() {
    digitalWrite(PWM_1, HIGH);
    digitalWrite(PWM_2, HIGH);

    // Increase speed gradually
    for (int i = 0; i <= 255; i++) {
        analogWrite(DIR_1, i);
        delay(120);
        Serial.println(i);
    }

    // Decrease speed gradually
    for (int i = 0; i <= 255; i++) {
        analogWrite(DIR_1, 255 - i);
        delay(120);
        Serial.println(255 - i);
    }

    while (1); // Stop execution
}
