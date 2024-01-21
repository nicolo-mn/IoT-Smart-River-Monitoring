#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin)
{
    servo.attach(pin);
}

void ServoMotor::setValveTo(int percentage)
{
    servo.write(percentage * OPEN_DEGREES / 100.0);
}