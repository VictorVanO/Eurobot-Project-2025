#include "Infrared.h"
#include "Arduino.h"

void initInfrared() {
    pinMode(LEFT_IR_SENSOR, INPUT);
    pinMode(RIGHT_IR_SENSOR, INPUT); 
}

// Black detected = HIGH
// White detected = LOW

// left black = black = high et right = white = low -> tourner à droite

int returnDirection() {
  int leftIR = digitalRead(LEFT_IR_SENSOR);
  int rightIR = digitalRead(RIGHT_IR_SENSOR);

  // Serial.println("left : ");
  // Serial.println(leftIR);
  // Serial.println("right : ");
  // Serial.println(rightIR);

  // If left = white && right = white -> Go Forward
  if (leftIR == HIGH && rightIR == HIGH) {
    // Both sensors on the line, move forward
    // Serial.println("FORWARD");
    return 0;
  }
  // If left = black && right = white -> Turn right
  else if (leftIR == LOW && rightIR == HIGH) { //**1
    // Left sensor on the line, turn left
    // Serial.println("RIGHT");
    return 1;
  }
  // If left = white && right = black -> Turn left
  else if (leftIR == HIGH && rightIR == LOW) {//**2
    // Right sensor on the line, turn right
    // Serial.println("LEFT");
    return 2;
  }
  // If left = black && right = black -> Stop
  // else if (leftIR == LOW && rightIR == LOW) {
  //   // Both sensors off the line, stop or reverse slightly
  //   // Serial.println("CHANGE POSITION");
  //   return 3;
  // }
  // delay(20); // Small delay for sensor stability
}

int returnDirection2() {// Read the values from the IR sensors : code works for turning 
  int leftIR = digitalRead(LEFT_IR_SENSOR);
  int rightIR = digitalRead(RIGHT_IR_SENSOR);
  Serial.println("left : ");
  Serial.println(leftIR);
  Serial.println("right : ");
  Serial.println(rightIR);
  if (leftIR == LOW && rightIR == LOW) {
    // Both sensors on the line, move forward
    Serial.println("FORWARD");
    return (0);
  }
  else if (leftIR == HIGH) {//**1
    // Left sensor on the line, turn left
    Serial.println("RIGHT");
    return(1);
  }
  else if (rightIR == HIGH) {//**2
    // Right sensor on the line, turn right
    Serial.println("LEFT");
    return(2);

  }
  else if (leftIR == HIGH && rightIR == HIGH) {
    // Both sensors off the line, stop or reverse slightly
    Serial.println("CHANGE POSITION");
    return(3);
  }
  delay(20); // Small delay for sensor stability
}