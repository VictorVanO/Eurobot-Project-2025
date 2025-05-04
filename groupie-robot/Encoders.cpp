#include "Encoders.h"


long leftCount = 0;
long rightCount = 0;


int lastLeftState = LOW;
int lastRightState = LOW;

void initEncoders() {
    pinMode(LEFT_ENCODER_PIN, INPUT);
    pinMode(RIGHT_ENCODER_PIN, INPUT);
}

void updateEncoders() {
    int currentLeft = digitalRead(LEFT_ENCODER_PIN);
    int currentRight = digitalRead(RIGHT_ENCODER_PIN);

    
    if (currentLeft == HIGH && lastLeftState == LOW) {
        leftCount++;
    }

    
    if (currentRight == HIGH && lastRightState == LOW) {
        rightCount++;
    }

    
    lastLeftState = currentLeft;
    lastRightState = currentRight;
}

long getLeftCount() {
    return leftCount;
}

long getRightCount() {
    return rightCount;
}

void resetCounts() {
    leftCount = 0;
    rightCount = 0;
}
