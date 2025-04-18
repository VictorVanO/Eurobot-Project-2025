#ifndef FSM_JAUNE_SUPERSTAR_H
#define FSM_JAUNE_SUPERSTAR_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Servomotor.h"
#include "SwitchSelector.h"
#include "FSM_common.h"
#include "Tirette.h"


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
    unsigned long directionTimer;
    unsigned long obstacleStart;
    unsigned long obstacleTotalTime;
    int motorSpeed;
    int motorSpeed1;
    int motorSpeed2;
    void handleState();
};

#endif
