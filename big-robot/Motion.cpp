// Motion.cpp - Non-blocking version of motion functions using FSM-style state updates

#include <Arduino.h>
#include "Motors.h"
#include "Encoders.h"
#include "Motion.h"

// State tracking
static unsigned long prevT = 0;
static float eprevR = 0, eprevL = 0;
static float eintegralR = 0, eintegralL = 0;
static long targetLeft = 0, targetRight = 0;
static bool motionActive = false;
static MotionType currentMotion = MOTION_NONE;

static int pmax = 235;
static int pmin = 25;

static int neutralZone = 15;
static int neutralTimeout = 200;

static bool leftDone = false, rightDone = false;
static unsigned long neutralStartL = 0, neutralStartR = 0;

// For GO_HOME recovery
static float resumeTargetLeft = 0;
static float resumeTargetRight = 0;
static bool resumeRightWheelOnly = false;
static bool resumeSmoothRotate = false;

void startGoStraight(float distance_cm) {
    resetEncoders();
    resetPIDVariables();
    currentMotion = MOTION_STRAIGHT;
    targetLeft = distance_cm * 80.0;
    targetRight = distance_cm * 80.0;
    resumeTargetLeft = targetLeft;
    resumeTargetRight = targetRight;
    resumeSmoothRotate = false;
    motionActive = true;
}

void startRotate(long angle_deg) {
    resetEncoders();
    resetPIDVariables();
    currentMotion = MOTION_ROTATE;
    float ticks = angle_deg * 14.75;
    targetLeft = ticks;
    targetRight = -ticks;
    resumeTargetLeft = targetLeft;
    resumeTargetRight = targetRight;
    resumeSmoothRotate = false;
    motionActive = true;
}

void startSmoothRotate(long angle_deg, bool rightWheel) {
    resetEncoders();
    resetPIDVariables();
    currentMotion = MOTION_SMOOTH_ROTATE;
    float ticks = abs(angle_deg) * 29.5; // Adjust this factor as needed
    if (rightWheel) {
        targetLeft = 0;
        targetRight = (angle_deg > 0 ? ticks : -ticks);
    } else {
        targetLeft = (angle_deg > 0 ? ticks : -ticks);
        targetRight = 0;
    }
    resumeTargetLeft = targetLeft;
    resumeTargetRight = targetRight;
    resumeRightWheelOnly = rightWheel;
    resumeSmoothRotate = true;
    motionActive = true;
}

void resumeMotion() {
    resetEncoders();
    resetPIDVariables();
    targetLeft = resumeTargetLeft;
    targetRight = resumeTargetRight;
    motionActive = true;
    if (resumeSmoothRotate) {
        currentMotion = MOTION_SMOOTH_ROTATE;
    } else if (resumeTargetLeft == resumeTargetRight) {
        currentMotion = MOTION_STRAIGHT;
    } else {
        currentMotion = MOTION_ROTATE;
    }
}

void updateMotion() {
    if (!motionActive) return;

    long currT = micros();
    float deltaT = (currT - prevT) / 1e6;
    prevT = currT;

    long posL = getEncoderLeft();
    long posR = getEncoderRight();

    long errL = targetLeft - posL;
    long errR = targetRight - posR;

    float dedtL = (errL - eprevL) / deltaT;
    float dedtR = (errR - eprevR) / deltaT;

    eintegralL += errL * deltaT;
    eintegralR += errR * deltaT;

    eintegralL = constrain(eintegralL, -300, 300);
    eintegralR = constrain(eintegralR, -300, 300);

    float kp = 1.0, ki = 0.07, kd = 0.0;

    float uL = kp * errL + ki * eintegralL + kd * dedtL;
    float uR = kp * errR + ki * eintegralR + kd * dedtR;

    int pwrL = constrain((int)fabs(uL), pmin, pmax);
    int pwrR = constrain((int)fabs(uR), pmin, pmax);

    int dirL = errL > 0 ? 1 : 0;
    int dirR = errR > 0 ? 1 : 0;

    // Neutral zone logic
    if (abs(errL) < neutralZone) {
        if (!leftDone) {
            if (neutralStartL == 0) neutralStartL = millis();
            if (millis() - neutralStartL > neutralTimeout) {
                pwrL = 0;
                dirL = 0;
                leftDone = true;
            }
        }
    } else {
        neutralStartL = 0;
        leftDone = false;
    }

    if (abs(errR) < neutralZone) {
        if (!rightDone) {
            if (neutralStartR == 0) neutralStartR = millis();
            if (millis() - neutralStartR > neutralTimeout) {
                pwrR = 0;
                dirR = 0;
                rightDone = true;
            }
        }
    } else {
        neutralStartR = 0;
        rightDone = false;
    }

    setMotor(dirL, pwrL, PWM_1, DIR_1);
    setMotor(dirR, pwrR, PWM_2, DIR_2);

    eprevL = errL;
    eprevR = errR;

    if (leftDone && rightDone) {
        stopMotors();
        motionActive = false;
        currentMotion = MOTION_NONE;
    }
}

void resetPIDVariables() {
    prevT = micros();
    eprevL = eprevR = 0;
    eintegralL = eintegralR = 0;
    leftDone = rightDone = false;
    neutralStartL = neutralStartR = 0;
}

bool isMotionComplete() {
    return !motionActive;
}

MotionType getCurrentMotionType() {
    return currentMotion;
}
