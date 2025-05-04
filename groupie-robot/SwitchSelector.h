#ifndef MODE_SELECTOR_H
#define MODE_SELECTOR_H

#include <Arduino.h>

class ModeSelector {
public:
    ModeSelector(int pin);
    void begin();
    bool isPrimaryMode();

private:
    int switchPin;
};


#endif
