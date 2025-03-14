#ifndef SERVO_ARMS_H
#define SERVO_ARMS_H

#include <Arduino.h>
#include <Servo.h>
#include <ServoEasing.h>

class ServoArms {
public:
    ServoArms();
    ~ServoArms();
    
    void init();
    
    void executeCommand(String command);
    void executeSequence(String commands[], int numCommands);
    
    void extendArms();
    void retractArms();
    void openHands(int openingLevel); // 0=closed, 1=half, 2=open
    void grabMaterials();
    void buildBleacher();
    
private:
    ServoEasing servo_x_axis;
    ServoEasing servo_y_left_axis;
    ServoEasing servo_y_right_axis;
    ServoEasing servo_z_axis;
    ServoEasing servo_r_axis;
    Servo clamp;
    
    int x_axis_degree;
    int y_axis_degree;
    int z_axis_degree;
    int r_axis_degree;
    
    int clamp_open_degree;
    int clamp_half_open_degree;
    int clamp_closed_degree;
    int general_speed;
    int low_speed;
    
    String armExtendCommands[5];
    String armRetractCommands[5];
    String grabMaterialsCommands[10];
    String buildBleacherCommands[10];
};

#endif