#ifndef FSM_JAUNE_H
#define FSM_JAUNE_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Infrared.h"
#include "Encoders.h"

#include "Tirette.h"
#include "FSM_commun.h"
#include "SwitchSelector.h"
#include "ServoLed.h"

class FSM_jaune {
public:
    FSM_jaune();
    void init();
    void run();
    void autoriserDemarrage();

private:
    RobotState state;
    bool demarrageAutorise = false;
    unsigned long startTime;
    unsigned long globalTimer;
    int motorSpeed;
    int avoidStep; 
    long ticksRotation; 
    void handleState();
    void evitementObstacle();
};

#endif
