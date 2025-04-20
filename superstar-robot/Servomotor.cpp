#include "Servomotor.h"
#include <Arduino.h>
#include <Servo.h>

Servo myServo;

const int durationParty = 500000;  
unsigned long partyStartTime;     

void initServoLed() {
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin, LOW);
    myServo.attach(11);
    myServo.write(startAngle);

}

void angle() {
  
  for (int angle = startAngle; angle <= endAngle; angle++) {
    myServo.write(angle);
    delay(stepDelay);
  }

  for (int angle = endAngle; angle >= startAngle; angle--) {
    myServo.write(angle);
    delay(stepDelay);
  }
}

void party() {
    partyStartTime = millis(); 
    while (millis() - partyStartTime < durationParty) { 
        angle();  
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
    }
    myServo.detach();  
}



