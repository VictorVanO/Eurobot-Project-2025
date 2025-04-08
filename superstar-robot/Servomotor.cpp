#include "Servomotor.h"
#include <Arduino.h>
#include <Servo.h>

Servo myServo;

const int durationParty = 10000;  
unsigned long partyStartTime;     

void initServoLed() {
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
        //digitalWrite(ledPin, HIGH);
        delay(1000);
        //digitalWrite(ledPin, LOW);
        delay(1000);
    }
    myServo.detach();  
}



