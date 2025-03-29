#include "Encoders.h"


// Variables de comptage
long leftCount = 0;
long rightCount = 0;

// État précédent pour détection de front montant
int lastLeftState = LOW;
int lastRightState = LOW;

void initEncoders() {
    pinMode(LEFT_ENCODER_PIN, INPUT);
    pinMode(RIGHT_ENCODER_PIN, INPUT);
}

void updateEncoders() {
    int currentLeft = digitalRead(LEFT_ENCODER_PIN);
    int currentRight = digitalRead(RIGHT_ENCODER_PIN);

    // Détection de front montant gauche
    if (currentLeft == HIGH && lastLeftState == LOW) {
        leftCount++;
    }

    // Détection de front montant droite
    if (currentRight == HIGH && lastRightState == LOW) {
        rightCount++;
    }

    // Mémorisation des états actuels
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