#include "LCD.h"

#define FIRST_MSG_LINE 1
#define SECOND_MSG_LINE 2
#define PROGRESS_BAR_LINE 2
#define PERCENTAGE_POS 9

LCD::LCD()
{
    displayLcd = new LiquidCrystal_I2C(0x27, 20, 4);
    displayLcd->init();
    displayLcd->backlight();
    displayLcd->noDisplay();
    percentage = 0;
}


void LCD::turnDisplayOn()
{
    displayLcd->display();
    displayLcd->backlight();
    displayLcd->clear();
}

void LCD::turnDisplayOff()
{
    displayLcd->noDisplay();
    displayLcd->noBacklight();
}

void LCD::setAutomatic()
{
    setValveTo(percentage);
    displayLcd->setCursor(0,SECOND_MSG_LINE);
    displayLcd->print("Automatic mode");
}

void LCD::setManual()
{
    setValveTo(percentage);
    displayLcd->setCursor(0,SECOND_MSG_LINE);
    displayLcd->print("Manual mode");
}

void LCD::setValveTo(int percentage)
{
    if (this->percentage == percentage) {
        return;
    }
    displayLcd->clear();
    displayLcd->setCursor(0, FIRST_MSG_LINE);
    displayLcd->print("Valve set to " + String(percentage) + "%");
    this->percentage = percentage;
}