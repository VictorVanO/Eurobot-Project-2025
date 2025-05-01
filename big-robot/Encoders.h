#ifndef ENCODERS_H
#define ENCODERS_H

void initEncoders();
void resetEncoders();
long getEncoderLeft();
long getEncoderRight();
void encoderLeft();
void encoderRight();

extern volatile long encoder_pos_left;
extern volatile long encoder_pos_right;

#endif
