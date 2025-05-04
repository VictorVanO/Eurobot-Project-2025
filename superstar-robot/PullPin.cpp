#include "PullPin.h"

PullPin::PullPin(uint8_t pin) {
  _pin = pin;
}

void PullPin::begin() {
  pinMode(_pin, INPUT_PULLUP);
}

bool PullPin::isActivated() {
  return digitalRead(_pin) == LOW;
}
