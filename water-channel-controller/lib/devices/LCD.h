#ifndef __LCD__
#define __LCD__

#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    LiquidCrystal_I2C *displayLcd;
    int percentage;
    bool isManualMode;
    void updateDisplay();

public:
    LCD();
    void setAutomatic();
    void setManual();
    void setValveTo(int percentage);
};

#endif
