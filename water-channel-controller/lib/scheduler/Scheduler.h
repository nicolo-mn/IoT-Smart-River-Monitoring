#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"

#define MAX_TASKS 10

class Scheduler
{
private:
    unsigned long basePeriod;
    int nTasks;
    Task *taskList[MAX_TASKS];


public:
    Scheduler(unsigned long basePeriod);
    bool addTask(Task *task);
    void sync();
    void schedule();
};

#endif