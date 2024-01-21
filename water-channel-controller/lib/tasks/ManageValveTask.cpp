#include <ManageValveTask.h>
#include <Arduino.h>
#include <config.h>

ManageValveTask::ManageValveTask(Button *btn, Potentiometer *pot, ServoMotor *servo, LCD *lcd)
{
    this->btn = btn;
    this->pot = pot;
    this->servo = servo;
    this->lcd = lcd;
    this->message = new String("");
    this->state = AUTOMATIC;
}

void ManageValveTask::tick()
{
    switch (state)
    {
    case AUTOMATIC:
        btn->sync();
        if (btn->isClicked())
        {
            state = MANUAL;
            lcd->setManual();
            Serial.println(MANUAL_MSG);
        }
        else
        {
            free(this->message);
            this->message = new String("");
            if (Serial.available() > 0)
            {
                message->concat(Serial.readStringUntil('\n'));
                if (this->message->indexOf(SET_VALVE_MSG) >= 0) {
                    int index = strlen(SET_VALVE_MSG);
                    String percentageString = this->message->substring(index);
                    int perc = percentageString.toInt();
                    servo->setValveTo(perc);
                    lcd->setValveTo(perc);
                }
            }
        }
        break;
    case MANUAL:
        btn->sync();
        pot->sync();
        if (btn->isClicked())
        {
            state = AUTOMATIC;
            lcd->setAutomatic();
            Serial.println(AUTOMATIC_MSG);
        }
        else
        {
            float perc = pot->getPercentage();
            servo->setValveTo(perc);
            lcd->setValveTo(perc);
        }
        break;
    default:
        break;
    }
}