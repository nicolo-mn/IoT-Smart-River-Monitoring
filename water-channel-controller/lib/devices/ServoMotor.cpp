#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin)
{
    servo.attach(pin);
    servo.write(CLOSED_DEGREES);
    currPercentage = 0;
}

void ServoMotor::setValveTo(int percentage)
{
    if (percentage == currPercentage) {
        return;
    } else if (percentage < 0) {
        setValveTo(0);
    } else if (percentage > 100) {
        setValveTo(100);
    } else {
        servo.write(percentage * OPEN_DEGREES / 100.0);
        currPercentage = percentage;
    }

}