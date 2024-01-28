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

            // Changes state to manual if button is clicked
            if (btn->isClicked())
            {
                state = MANUAL;
                lcd->setManual();

                JsonDocument doc;
                doc[TYPE] = CHANGE_MODE;
                doc[MODE] = "MANUAL";
                String jsonMessage;
                serializeJson(doc, jsonMessage);
                Serial.println(jsonMessage);
                fflush(stdout);
            }
            else if (Serial.available() > 0)
            {
                // Read the incoming data
                *this->message = "";
                message->concat(Serial.readStringUntil('\n'));
                JsonDocument doc;
                deserializeJson(doc, *message);
                if (doc.containsKey(TYPE) && doc[TYPE] == "SET_VALVE_AUTOMATIC" && doc.containsKey(VALVE_OPENING))
                {
                    int perc = doc[VALVE_OPENING];
                    servo->setValveTo(perc);
                    lcd->setValveTo(perc);
                }
            }
            break;
        case MANUAL:
            btn->sync();
            pot->sync();
            // Changes state to automatic if button is clicked
            if (btn->isClicked())
            {
                state = AUTOMATIC;
                lcd->setAutomatic();
                JsonDocument doc;
                doc[TYPE] = CHANGE_MODE;
                doc[MODE] = "AUTOMATIC";
                String jsonMessage;
                serializeJson(doc, jsonMessage);
                Serial.println(jsonMessage);
                fflush(stdout);
            }
            else
            {
                // Updates valve opening if potentiometer has changed
                if (pot->hasChanged()) {
                    int perc = pot->getPercentage();
                    servo->setValveTo(perc);
                    lcd->setValveTo(perc);

                    JsonDocument doc;
                    doc[TYPE] = "SET_VALVE_MANUAL";
                    doc[VALVE_OPENING] = perc;
                    String jsonMessage;
                    serializeJson(doc, jsonMessage);
                    Serial.println(jsonMessage);
                    fflush(stdout);
                }
            }
            break;
        default:
            break;
    }
}