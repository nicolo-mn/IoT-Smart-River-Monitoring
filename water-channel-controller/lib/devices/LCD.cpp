#include <Arduino.h>
#include "LCD.h"

#define FIRST_MSG_LINE 1
#define SECOND_MSG_LINE 2
#define PROGRESS_BAR_LINE 2
#define PERCENTAGE_POS 9

LCD::LCD()
{
    displayLcd = new LiquidCrystal_I2C(0x27, 20, 4);
    displayLcd->init();
    displayLcd->display();
    displayLcd->backlight();
    displayLcd->clear();
    percentage = 50;
    isManualMode = false;
    updateDisplay();
}

void LCD::setAutomatic()
{
    if (!isManualMode) {
        return;
    }
    isManualMode = false;
    updateDisplay();
}

void LCD::setManual()
{
    if (isManualMode) {
        return;
    }
    isManualMode = true;
    updateDisplay();
}

void LCD::setValveTo(int percentage)
{
    if (this->percentage == percentage) {
        return;
    } else if (percentage < 0) {
        percentage = 0;
    } else if (percentage > 100) {
        percentage = 100;
    } else {
        this->percentage = percentage;
    }
    updateDisplay();
}

void LCD::updateDisplay()
{
    displayLcd->clear();
    displayLcd->setCursor(0, FIRST_MSG_LINE);
    displayLcd->print("Valve set to " + String(percentage) + "%");
    displayLcd->setCursor(0, SECOND_MSG_LINE);
    String mode = isManualMode ? "Manual" : "Automatic";
    displayLcd->print(mode + " mode");
}