#ifndef FSM_BLEU_SUPERSTAR_H
#define FSM_BLEU_SUPERSTAR_H

#include "Motors.h"
#include "Arduino.h"
#include "Ultrasonic.h"
#include "Servomotor.h"
#include "ButtonUrgence.h"
#include "SwitchSelector.h"
#include "FSM_common.h"
#include "Tirette.h"


class FSM_bleu {
public:
    FSM_bleu();
    void init();
    void run();

private:
    RobotState state;
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
