#include <Arduino.h>
#include "Motors.h"
#include "Encoders.h"
#include "Motion.h"

// PID variables
int prevT = 0;
float eprevR = 0;
float eprevL = 0;
float eintegralR = 0;
float eintegralL = 0;

int pmax = 235;
int pmin = 25;

unsigned long neutralStartTimeR = 0;
unsigned long neutralStartTimeL = 0;
bool inNeutralZoneR = false;
bool inNeutralZoneL = false;

void resetPIDVariables() {
    eprevR = 0;
    eprevL = 0;
    eintegralR = 0;
    eintegralL = 0;
    inNeutralZoneR = false;
    inNeutralZoneL = false;
    resetEncoders();
}

void GoStraight(float target_cm) {
    float target_R = target_cm * 80.0;
    float target_L = target_cm * 80.0;

    float kp = 1.0, kd = 0.0, ki = 0.07;

    resetPIDVariables();

    int neutralZone = 15;
    int neutralTimeout = 150;

    bool rightStopped = false;
    bool leftStopped = false;

    while (true) {
        long currT = micros();
        float deltaT = (currT - prevT) / 1e6;
        prevT = currT;

        long e_R = target_R - getEncoderRight();
        long e_L = target_L - getEncoderLeft();

        float dedtR = (e_R - eprevR) / deltaT;
        float dedtL = (e_L - eprevL) / deltaT;

        eintegralR += e_R * deltaT;
        eintegralL += e_L * deltaT;

        eintegralR = constrain(eintegralR, -300, 300);
        eintegralL = constrain(eintegralL, -300, 300);

        float uR = kp * e_R + kd * dedtR + ki * eintegralR;
        float uL = kp * e_L + kd * dedtL + ki * eintegralL;

        int pwrR = constrain((int)fabs(uR), pmin, pmax);
        int pwrL = constrain((int)fabs(uL), pmin, pmax);

        int dirR = e_R > 0 ? 1 : 0;
        int dirL = e_L > 0 ? 1 : 0;

        if (abs(e_R) < neutralZone) {
            if (!inNeutralZoneR) {
                neutralStartTimeR = millis();
                inNeutralZoneR = true;
            } else if (millis() - neutralStartTimeR > neutralTimeout) {
                pwrR = 0;
                dirR = 0;
                rightStopped = true;
            }
        } else inNeutralZoneR = false;

        if (abs(e_L) < neutralZone) {
            if (!inNeutralZoneL) {
                neutralStartTimeL = millis();
                inNeutralZoneL = true;
            } else if (millis() - neutralStartTimeL > neutralTimeout) {
                pwrL = 0;
                dirL = 0;
                leftStopped = true;
            }
        } else inNeutralZoneL = false;

        setMotor(dirL, pwrL, PWM_1, DIR_1);
        setMotor(dirR, pwrR, PWM_2, DIR_2);

        eprevR = e_R;
        eprevL = e_L;

        if (rightStopped && leftStopped) {
            stopMotors();
            return;
        }
    }
}

void Rotate(long angle_deg) {
    const float TICKS_PER_DEGREE = 14.75;
    long target_R = -angle_deg * TICKS_PER_DEGREE;
    long target_L =  angle_deg * TICKS_PER_DEGREE;

    float kp = 1.0, kd = 0.0, ki = 0.07;

    resetPIDVariables();

    int neutralZone = 15;
    int neutralTimeout = 200;

    bool rightStopped = false;
    bool leftStopped = false;

    while (true) {
        long currT = micros();
        float deltaT = (currT - prevT) / 1e6;
        prevT = currT;

        long e_R = target_R - getEncoderRight();
        long e_L = target_L - getEncoderLeft();

        float dedtR = (e_R - eprevR) / deltaT;
        float dedtL = (e_L - eprevL) / deltaT;

        eintegralR += e_R * deltaT;
        eintegralL += e_L * deltaT;

        eintegralR = constrain(eintegralR, -300, 300);
        eintegralL = constrain(eintegralL, -300, 300);

        float uR = kp * e_R + kd * dedtR + ki * eintegralR;
        float uL = kp * e_L + kd * dedtL + ki * eintegralL;

        int pwrR = constrain((int)fabs(uR), pmin, pmax);
        int pwrL = constrain((int)fabs(uL), pmin, pmax);

        int dirR = e_R > 0 ? 1 : 0;
        int dirL = e_L > 0 ? 1 : 0;

        if (abs(e_R) < neutralZone) {
            if (!inNeutralZoneR) {
                neutralStartTimeR = millis();
                inNeutralZoneR = true;
            } else if (millis() - neutralStartTimeR > neutralTimeout) {
                pwrR = 0;
                dirR = 0;
                rightStopped = true;
            }
        } else inNeutralZoneR = false;

        if (abs(e_L) < neutralZone) {
            if (!inNeutralZoneL) {
                neutralStartTimeL = millis();
                inNeutralZoneL = true;
            } else if (millis() - neutralStartTimeL > neutralTimeout) {
                pwrL = 0;
                dirL = 0;
                leftStopped = true;
            }
        } else inNeutralZoneL = false;

        setMotor(dirL, pwrL, PWM_1, DIR_1);
        setMotor(dirR, pwrR, PWM_2, DIR_2);

        eprevR = e_R;
        eprevL = e_L;

        if (rightStopped && leftStopped) {
            stopMotors();
            return;
        }
    }
}

void smoothRotate(long angle_deg, bool rightWheel) {
    const float TICKS_PER_DEGREE = 29.5; // À ajuster selon ton robot
    long targetTicks = abs(angle_deg) * TICKS_PER_DEGREE;

    resetPIDVariables();

    float kp = 1.0, kd = 0.0, ki = 0.07;
    int neutralZone = 15;
    int neutralTimeout = 150;

    bool motorStopped = false;
    unsigned long neutralStartTime = 0;
    bool inNeutralZone = false;

    while (true) {
        long currT = micros();
        float deltaT = ((float)(currT - prevT)) / 1e6;
        prevT = currT;

        int e = 0;
        float dedt = 0;
        float eintegral = 0;
        float u = 0;
        int pwr = 0;
        int dir = 1;

        if (rightWheel) {
            e = (angle_deg > 0 ? targetTicks : -targetTicks) - getEncoderRight();
            dedt = (e - eprevR) / deltaT;
            eintegral = eintegralR + e * deltaT;
            eintegral = constrain(eintegral, -300, 300);
            eintegralR = eintegral;
            u = kp * e + kd * dedt + ki * eintegral;
            pwr = constrain((int)fabs(u), pmin, pmax);
            dir = e > 0 ? 1 : 0;

            if (abs(e) <= neutralZone) {
                if (!inNeutralZone) {
                    neutralStartTime = millis();
                    inNeutralZone = true;
                } else if ((millis() - neutralStartTime) > neutralTimeout) {
                    dir = 0;
                    pwr = 0;
                    motorStopped = true;
                }
            } else inNeutralZone = false;

            setMotor(dir, pwr, PWM_2, DIR_2);
            setMotor(0, 0, PWM_1, DIR_1);
            eprevR = e;

        } else {
            e = (angle_deg > 0 ? targetTicks : -targetTicks) - getEncoderLeft();
            dedt = (e - eprevL) / deltaT;
            eintegral = eintegralL + e * deltaT;
            eintegral = constrain(eintegral, -300, 300);
            eintegralL = eintegral;
            u = kp * e + kd * dedt + ki * eintegral;
            pwr = constrain((int)fabs(u), pmin, pmax);
            dir = e > 0 ? 1 : 0;

            if (abs(e) <= neutralZone) {
                if (!inNeutralZone) {
                    neutralStartTime = millis();
                    inNeutralZone = true;
                } else if ((millis() - neutralStartTime) > neutralTimeout) {
                    dir = 0;
                    pwr = 0;
                    motorStopped = true;
                }
            } else inNeutralZone = false;

            setMotor(dir, pwr, PWM_1, DIR_1);
            setMotor(0, 0, PWM_2, DIR_2);
            eprevL = e;
        }

        if (motorStopped) {
            stopMotors();
            return;
        }
    }
}