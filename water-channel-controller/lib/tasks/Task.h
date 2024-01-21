#ifndef __TASK__
#define __TASK__

#include <Arduino.h>
class Task
{
private:
    unsigned long taskPeriod;
    unsigned long timeElapsed;

public:

    virtual void init(unsigned long period)
    {
        this->taskPeriod = period;
        this->timeElapsed = 0;
    }

    virtual void tick() = 0;

    bool updateAndCheckTime(unsigned long basePeriod)
    {
        this->timeElapsed += basePeriod;
        if (this->timeElapsed >= this->taskPeriod)
        {
            timeElapsed = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif