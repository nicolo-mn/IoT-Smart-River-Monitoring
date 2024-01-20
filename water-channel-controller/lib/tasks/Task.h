#ifndef __TASK__
#define __TASK__

#include <Arduino.h>
#include <FlagManager.h>
class Task
{
protected:
    FlagManager *flagManager;

private:
    unsigned long taskPeriod;
    unsigned long timeElapsed;

public:
    Task(FlagManager *flagManager)
    {
        this->flagManager = flagManager;
    }

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