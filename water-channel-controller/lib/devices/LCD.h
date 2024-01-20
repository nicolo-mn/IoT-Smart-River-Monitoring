#ifndef __LCD__
#define __LCD__

#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    LiquidCrystal_I2C *displayLcd;
    void showOneLineMessage(String message);
    void showTwoLinesMessage(String firstLine, String secondLine);

public:
    LCD();
    void turnDisplayOn();
    void turnDisplayOff();
    void showWelcomeMessage();
    void showEnteringMessage();
    void showReadyToWashMessage();
    void updateProgressBar(unsigned long count);
    void showWashingMessage();
    void showWashingCompleteMessage();
    void showErrorMessage();
};

#endif
