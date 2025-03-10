#include "LCD.h"
#include <Arduino.h>

LCD::LCD() {
    // Using the 0x3F address from your sample code
    lcd = new LiquidCrystal_I2C(0x3F, 20, 4);
}

LCD::~LCD() {
    delete lcd;
}

void LCD::init() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(3, 0);
    lcd->print("Spice Bot");
    lcd->setCursor(2, 1);
    lcd->print("Démarrage...");
    delay(1000); // Display startup message for 2 seconds
}

void LCD::clear() {
    lcd->clear();
}

void LCD::printLine(int line, const char* message) {
    lcd->setCursor(0, line);
    lcd->print("                    "); // Clear the line (20 spaces)
    lcd->setCursor(0, line);
    lcd->print(message);
}

void LCD::printLineWithInt(int line, const char* message, int value) {
    lcd->setCursor(0, line);
    lcd->print("                    "); // Clear the line
    lcd->setCursor(0, line);
    lcd->print(message);
    lcd->print(value);
}

void LCD::printLineWithFloat(int line, const char* message, float value, int precision) {
    lcd->setCursor(0, line);
    lcd->print("                    "); // Clear the line
    lcd->setCursor(0, line);
    lcd->print(message);
    lcd->print(value, precision);
}

void LCD::displayState(const char* stateName) {
    printLine(0, "State:");
    printLine(1, stateName);
}

void LCD::displaySensorData(float* distances, int numSensors) {
    printLine(2, "Sensors:");
    
    String sensorData = "";
    for (int i = 0; i < numSensors && i < 3; i++) { // Display up to 3 sensors
        if (i > 0) sensorData += " ";
        sensorData += String(distances[i], 1);
    }
    
    printLine(3, sensorData.c_str());
}

void LCD::backlight(bool on) {
    if (on) {
        lcd->backlight();
    } else {
        lcd->noBacklight();
    }
}