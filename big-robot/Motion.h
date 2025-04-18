#ifndef MOTION_H
#define MOTION_H

enum MotionType {
    MOTION_NONE,
    MOTION_STRAIGHT,
    MOTION_ROTATE,
    MOTION_SMOOTH_ROTATE
};

void startGoStraight(float distance_cm);
void startRotate(long angle_deg);
void startSmoothRotate(long angle_deg, bool rightWheel);
void updateMotion();
bool isMotionComplete();
void resetPIDVariables();
MotionType getCurrentMotionType();

#endif