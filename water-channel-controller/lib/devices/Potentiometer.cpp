#include "Potentiometer.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin)
{
    this->pin = pin;
    this->percentageValue = 0;
}

void Potentiometer::sync()
{
    int newValue = analogRead(pin) / 1023.0 * 100.0;
    if (newValue != percentageValue) {
        percentageValue = newValue;
        hasChangedFlag = true;
    } else {
        hasChangedFlag = false;
    }
}

int Potentiometer::getPercentage()
{
    return percentageValue;
}

bool Potentiometer::hasChanged()
{
    return hasChangedFlag;
}
