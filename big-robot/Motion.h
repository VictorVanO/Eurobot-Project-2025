#ifndef MOTION_H
#define MOTION_H

void GoStraight(float distance_cm);
void Rotate(long angle_deg);
void smoothRotate(long angle_deg, bool rightWheel);
void resetPIDVariables();

#endif
