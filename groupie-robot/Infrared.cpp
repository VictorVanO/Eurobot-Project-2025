#include "Infrared.h"
#include "Arduino.h"

void initInfrared() {
    pinMode(LEFT_IR_SENSOR, INPUT);
    pinMode(RIGHT_IR_SENSOR, INPUT); 
}

int returnDirection() {
  int leftIR = digitalRead(LEFT_IR_SENSOR);   // 1 = black
  int rightIR = digitalRead(RIGHT_IR_SENSOR); // 1 = black

  // Cas 1 : 
  if (leftIR == HIGH && rightIR == HIGH) {
    return 0; // Go forward
  }

  // Cas 2 :
  else if (leftIR == HIGH && rightIR == LOW) {
    return 1; // Turn right
  }

  // Cas 3 : 
  else if (leftIR == LOW && rightIR == HIGH) {
    return 2; // Turn right
  }

  // Cas 4 : 
  else if (leftIR == LOW && rightIR == LOW) {
    return 3;  
  }
}
