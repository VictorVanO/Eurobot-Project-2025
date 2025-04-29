#include "LCD.h"
#include <Arduino.h>

LCD::LCD() {
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
    lcd->print("Total points");
    lcd->setCursor(2, 1);
    lcd->print("0.");
}

void LCD::clear() {
    lcd->clear();
}

void LCD::printLine(int line, const char* message) {
    lcd->setCursor(0, line);
    lcd->print("                    ");
    lcd->setCursor(0, line);
    lcd->print(message);
}

void LCD::printLineWithInt(int line, const char* message, int value) {
    lcd->setCursor(0, line);
    lcd->print("                    ");
    lcd->setCursor(0, line);
    lcd->print(message);
    lcd->print(value);
}

void LCD::printLineWithFloat(int line, const char* message, float value, int precision) {
    lcd->setCursor(0, line);
    lcd->print("                    ");
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
    for (int i = 0; i < numSensors && i < 3; i++) {
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