#include "Infrared.h"
#include "Arduino.h"

void initInfrared() {
    pinMode(LEFT_IR_SENSOR, INPUT);
    pinMode(RIGHT_IR_SENSOR, INPUT); 
}


int returnDirection() {
  int leftIR = digitalRead(LEFT_IR_SENSOR);   // 1 = noir
  int rightIR = digitalRead(RIGHT_IR_SENSOR); // 1 = noir

  // Cas 1 : les deux capteurs détectent du noir → AVANCER
  if (leftIR == HIGH && rightIR == HIGH) {
    return 0; // Avancer
  }

  // Cas 2 : gauche = noir, droite = blanc → TOURNER DROITE
  else if (leftIR == HIGH && rightIR == LOW) {
    return 1; // Tourner à droite
  }

  // Cas 3 : gauche = blanc, droite = noir → TOURNER GAUCHE
  else if (leftIR == LOW && rightIR == HIGH) {
    return 2; // Tourner à gauche
  }
}