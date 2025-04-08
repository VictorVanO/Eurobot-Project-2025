#include "Tirette.h"

Tirette::Tirette(uint8_t pin) {
  _pin = pin;
}

void Tirette::begin() {
  pinMode(_pin, INPUT_PULLUP);
}

bool Tirette::estActivee() {
  return digitalRead(_pin) == LOW; 
}
