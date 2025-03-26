#include "ServoArms.h"

ServoArms::ServoArms() : 
    x_axis_degree(90), 
    y_axis_degree(120), 
    z_axis_degree(40), 
    r_axis_degree(110),
    clamp_open_degree(120), 
    clamp_half_open_degree(60), 
    clamp_closed_degree(35),
    general_speed(38), 
    low_speed(20) {
    
    // Commands to extend arms
    armExtendCommands[0] = "Y=23,Z=85,R=160";
    armExtendCommands[1] = "C=2"; // Open clamp
    
    // Commands to retract arms
    armRetractCommands[0] = "Y=120,Z=40,R=110";
    armRetractCommands[1] = "C=0"; // Close clamp
    
    // Commands for grabbing materials
    grabMaterialsCommands[0] = "X=50";
    grabMaterialsCommands[1] = "Y=35,Z=75,R=150";
    grabMaterialsCommands[2] = "C=1"; // Half open clamp
    grabMaterialsCommands[3] = "Y=120,Z=40,R=110";
    
    // Commands for building bleacher
    buildBleacherCommands[0] = "X=90";
    buildBleacherCommands[1] = "Y=40,Z=78,R=150";
    buildBleacherCommands[2] = "C=2"; // Open clamp
    buildBleacherCommands[3] = "Y=120,Z=40,R=110";
}

ServoArms::~ServoArms() {
    // Cleanup if needed
}

void ServoArms::init() {
    servo_y_left_axis.setReverseOperation(true);
    servo_x_axis.attach(52, x_axis_degree);
    servo_y_left_axis.attach(51, y_axis_degree);
    servo_y_right_axis.attach(40, y_axis_degree);
    servo_z_axis.attach(9, z_axis_degree);
    servo_r_axis.attach(10, r_axis_degree);
    clamp.attach(50);
    clamp.write(clamp_closed_degree);
    
    setEasingTypeForAllServos(EASE_SINE_IN_OUT);
    delay(1000);  // Wait for servo to reach start position
}

void ServoArms::executeCommand(String command) {
    int index = 0;
    while ((index = command.indexOf(',')) > 0) {
        String subcommand = command.substring(0, index);
        command = command.substring(index + 1);
        subcommand.trim();
        executeCommand(subcommand);
    }
    
    String axis = command.substring(0, 1);
    int value = command.substring(2).toInt();
    
    if (axis == "X" || axis == "x") {
        int speed = (abs(value - x_axis_degree) <= 20) ? low_speed : general_speed;
        servo_x_axis.setEaseTo(value, general_speed);
        x_axis_degree = value;
    } else if (axis == "Y" || axis == "y") {
        int speed = (abs(value - y_axis_degree) <= 20) ? low_speed : general_speed;
        servo_y_left_axis.setEaseTo(value - 10, speed);
        servo_y_right_axis.setEaseTo(value - 10, speed);
        y_axis_degree = value;
    } else if (axis == "Z" || axis == "z") {
        int speed = (abs(value - z_axis_degree) <= 20) ? low_speed : general_speed;
        servo_z_axis.setEaseTo(value, speed);
        z_axis_degree = value;
    } else if (axis == "R" || axis == "r") {
        int speed = (abs(value - r_axis_degree) <= 20) ? low_speed : general_speed;
        servo_r_axis.setEaseTo(value, speed);
        r_axis_degree = value;
    } else if (axis == "C" || axis == "c") {
        clamp.write(value == 2 ? clamp_open_degree : (value == 1 ? clamp_half_open_degree : clamp_closed_degree));
    }
}

void ServoArms::executeSequence(String commands[], int numCommands) {
    for (int i = 0; i < numCommands; i++) {
        executeCommand(commands[i]);
        synchronizeAllServosStartAndWaitForAllServosToStop();
        delay(500); // Small delay between commands
    }
}

void ServoArms::extendArms() {
    executeSequence(armExtendCommands, 2);
}

void ServoArms::retractArms() {
    executeSequence(armRetractCommands, 2);
}

void ServoArms::openHands(int openingLevel) {
    String command = "C=";
    command += openingLevel;
    executeCommand(command);
    synchronizeAllServosStartAndWaitForAllServosToStop();
}

void ServoArms::grabMaterials() {
    executeSequence(grabMaterialsCommands, 4);
}

void ServoArms::buildBleacher() {
    executeSequence(buildBleacherCommands, 4);
}