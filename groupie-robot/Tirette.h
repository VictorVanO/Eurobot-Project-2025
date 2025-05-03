#ifndef TIRETTE_H
#define TIRETTE_H

#include <Arduino.h>

class Tirette {
  public:
    Tirette(uint8_t pin);     
    void begin();             
    bool estActivee();        
  private:
    uint8_t _pin;
};

#endif
