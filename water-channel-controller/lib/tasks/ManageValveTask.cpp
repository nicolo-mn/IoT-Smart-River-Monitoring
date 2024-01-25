#include <ManageValveTask.h>
#include <Arduino.h>
#include <ArduinoJson.h>
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

            JsonDocument doc;
            doc[TYPE] = MODE_TYPE;
            doc[MODE] = AUTOMATIC_MSG;
            String jsonMessage;
            serializeJson(doc, jsonMessage);
            Serial.println(jsonMessage);
            // Serial.println(MANUAL_MSG);
            // fflush(stdout);
        }
        else
        {
            if (Serial.available() > 0)
            {
                *this->message = "";
                message->concat(Serial.readStringUntil('\n'));
                Serial.println("ricevuto" + *message);

                JsonDocument doc;
                deserializeJson(doc, *message);

                if (doc.containsKey(TYPE) && doc[TYPE] == SET_VALVE_TYPE && doc.containsKey(VALVE_OPENING))
                {
                    int perc = doc[VALVE_OPENING];
                    servo->setValveTo(perc);
                    lcd->setValveTo(perc);
                }

                // if (this->message->indexOf(SET_VALVE_MSG) >= 0) {
                //     int index = strlen(SET_VALVE_MSG);
                //     String percentageString = this->message->substring(index);
                //     int perc = percentageString.toInt();
                //     servo->setValveTo(perc);
                //     lcd->setValveTo(perc);
                // }
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
            JsonDocument doc;
            doc[TYPE] = MODE_TYPE;
            doc[MODE] = AUTOMATIC_MSG;
            String jsonMessage;
            serializeJson(doc, jsonMessage);
            Serial.println(jsonMessage);
            // Serial.println(AUTOMATIC_MSG);
            // fflush(stdout);
        }
        else
        {
            if (pot->hasChanged()) {
                int perc = pot->getPercentage();
                servo->setValveTo(perc);
                lcd->setValveTo(perc);

                JsonDocument doc;
                doc[TYPE] = SET_VALVE_TYPE;
                doc[VALVE_OPENING] = perc;
                String jsonMessage;
                serializeJson(doc, jsonMessage);
                Serial.println(jsonMessage);
            }
        }
        break;
    default:
        break;
    }
}