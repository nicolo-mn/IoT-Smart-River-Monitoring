#ifndef __MANAGE_VALVE_TASK__
#define __MANAGE_VALVE_TASK__

#include "Task.h"
#include <Button.h>
#include <Potentiometer.h>
#include <ServoMotor.h>
#include <LCD.h>

class ManageValveTask : public Task
{
    private:
        Button *btn;
        Potentiometer *pot;
        ServoMotor *servo;
        LCD *lcd;
        String *message;
        enum
        {
            AUTOMATIC,
            MANUAL
        } state;

    public:
        ManageValveTask(Button *btn, Potentiometer *pot, ServoMotor *servo, LCD *lcd);
        void tick();
};

#endif