#ifndef MODE_SELECTOR_H
#define MODE_SELECTOR_H

#include <Arduino.h>

class ModeSelector {
public:
    ModeSelector(int pin);
    void begin();
    bool isPrimaryMode(); // true si position vers FSM actuelle

private:
    int switchPin;
};

#endif

