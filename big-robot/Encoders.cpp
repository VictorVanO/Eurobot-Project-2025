#include <Arduino.h>
#include "Encoders.h"

#define ENC_A 18
#define ENC_B 19
#define ENC_C 3
#define ENC_D 2

volatile long encoder_pos_left = 0;
volatile long encoder_pos_right = 0;

void initEncoders() {
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);
    pinMode(ENC_C, INPUT);
    pinMode(ENC_D, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENC_B), encoderLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(ENC_D), encoderRight, RISING);
}

void encoderLeft() {
    if (digitalRead(ENC_A) == HIGH) encoder_pos_left++;
    else encoder_pos_left--;
}

void encoderRight() {
    if (digitalRead(ENC_C) == HIGH) encoder_pos_right++;
    else encoder_pos_right--;
}

void resetEncoders() {
    encoder_pos_left = 0;
    encoder_pos_right = 0;
}

long getEncoderLeft() {
    return encoder_pos_left;
}

long getEncoderRight() {
    return encoder_pos_right;
}
