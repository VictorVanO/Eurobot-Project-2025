#include "Motors.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);

void initMotors() {
  AFMS.begin();
}

void setMotorsSpeed(uint8_t speed) {
  motor1->setSpeed(speed);
  motor2->setSpeed(speed - 4);
}

void runMotors(uint8_t direction) {
  motor1->run(direction);
  motor2->run(direction);
}

void turnLeft() {
    motor1->run(RELEASE);
    motor2->run(FORWARD);
}

void goForward(){
    motor1->run(FORWARD);
    motor2->run(FORWARD);
}

void turnRight() {
    motor1->run(FORWARD);
    motor2->run(RELEASE);
}

void stopMotors() {  
    motor1->run(RELEASE);
    motor2->run(RELEASE);
}