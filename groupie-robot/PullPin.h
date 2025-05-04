#ifndef PULL_PIN_H
#define PULL_PIN_H

#include <Arduino.h>

class PullPin {
  public:
    PullPin(uint8_t pin);       // Constructor
    void begin();               // Initializes the pin
    bool isActivated();        // Returns true if the pin is activated (pulled)
  private:
    uint8_t _pin;
};

#endif
