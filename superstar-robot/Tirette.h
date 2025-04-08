#ifndef TIRETTE_H
#define TIRETTE_H

#include <Arduino.h>

class Tirette {
  public:
    Tirette(uint8_t pin);     // Constructeur avec la pin à laquelle la tirette est connectée
    void begin();             // Initialise la pin en mode INPUT_PULLUP
    bool estActivee();        // Retourne true si la tirette est tirée (contact fermé)

  private:
    uint8_t _pin;
};

#endif
