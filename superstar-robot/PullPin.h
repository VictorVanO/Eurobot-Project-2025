#ifndef PULLPIN_H
#define PULLPIN_H

#include <Arduino.h>

// Class to manage a pull-pin (physical trigger to start the robot)
class PullPin {
  public:
    PullPin(uint8_t pin);       // Constructor: takes the pin connected to the pull-pin
    void begin();               // Initializes the pin mode
    bool isActivated();         // Returns true if the pull-pin has been pulled

  private:
    uint8_t _pin;               // Pin number connected to the pull-pin
};

#endif
