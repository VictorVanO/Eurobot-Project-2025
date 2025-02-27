#include "Servomotor.h"
#include <Arduino.h>
#include <Servo.h>

Servo myServo;

const int durationParty = 10000;  
unsigned long partyStartTime;     

void initServoLed() {
    //pinMode(ledPin, OUTPUT);
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
    Serial.println("TIME TO PARTYYY 🎉");
    partyStartTime = millis();  // Stocker le temps de début

    while (millis() - partyStartTime < durationParty) {  // Boucle pendant 10 sec
        angle();  // Déplacement du servo
        //digitalWrite(ledPin, HIGH);
        //delay(1000);
        //digitalWrite(ledPin, LOW);
        //delay(1000);
    }

    Serial.println("END OF THE PARTY 🎭");
    myServo.detach();  
}



