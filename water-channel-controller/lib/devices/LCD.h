#ifndef __LCD__
#define __LCD__

#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    LiquidCrystal_I2C *displayLcd;
    int percentage;

public:
    LCD();
    void turnDisplayOn();
    void turnDisplayOff();
    void setAutomatic();
    void setManual();
    void setValveTo(int percentage);
};

#endif
