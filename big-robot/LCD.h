#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

class LCD {
private:
    LiquidCrystal_I2C* lcd;
    
public:
    LCD();
    ~LCD();
    
    void init();
    void clear();
    void printLine(int line, const char* message);
    void printLineWithInt(int line, const char* message, int value);
    void printLineWithFloat(int line, const char* message, float value, int precision = 2);
    void displayState(const char* stateName);
    void displaySensorData(float* distances, int numSensors);
    void backlight(bool on = true);
};

#endif