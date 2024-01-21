#ifndef __SERVOMOTOR__
#define __SERVOMOTOR__

#include <Servo.h>

#define OPEN_DEGREES 180
#define CLOSED_DEGREES 0

class ServoMotor
{
private:
    Servo servo;
    int currPercentage;
public:
    ServoMotor(int pin);
    void setValveTo(int percentage);
};

#endif
