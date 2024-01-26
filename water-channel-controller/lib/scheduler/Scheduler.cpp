#include "Scheduler.h"
#include <Arduino.h>    

volatile bool timerFlag;

Scheduler::Scheduler(unsigned long basePeriod) 
{
    this->basePeriod = basePeriod;
    nTasks = 0;
}

bool Scheduler::addTask(Task *task)
{
    if (nTasks < MAX_TASKS - 1)
    {
        taskList[nTasks] = task;
        nTasks++;
        return true;
    }
    else
    {
        return false;
    }
}

void Scheduler::sync()
{
    delay(this->basePeriod - (millis() % this->basePeriod));
}

void Scheduler::schedule()
{
    unsigned long tstart = millis();

    for (int i = 0; i < nTasks; i++)
    {
        if (taskList[i]->updateAndCheckTime(basePeriod))
        {
            taskList[i]->tick();
        }
    }

    unsigned long telapsed = millis() - tstart;
    if (telapsed > basePeriod) {
        Serial.println("Scheduler Error: tasks takes too long!!! elapsed: " + String(telapsed));
    }

    sync();
}