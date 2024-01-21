/**
 * Implemented by 
 * Livia Cardaccia, Nicolò Monaldini,
 * Giovanni Prete, Matteo Sartini
*/

#include <Arduino.h>
#include <LCD.h>
#include <ServoMotor.h>
#include <Potentiometer.h>
#include <Scheduler.h>
#include <config.h>
#include <FlagManager.h>

FlagManager *flagManager;
LCD *displayLcd;
Potentiometer *pot;
Scheduler *sched;
ServoMotor *servo;

void setup()
{
    Serial.begin(9600);
    flagManager = new FlagManager();
    sched = new Scheduler(SCHEDULER_PERIOD);
    displayLcd = new LCD();
    displayLcd->turnDisplayOn();
    servo = new ServoMotor(SERVO_PIN);
    pot = new Potentiometer(POT_PIN);
    // task1->init(DETECTION_PERIOD);
    // sched->addTask(task1);
}

void loop()
{
    // sched->schedule();
    // displayLcd->setValveTo(0);
    // delay(2000);
    // displayLcd->setValveTo(100);
    // delay(2000);
    // displayLcd->setAutomatic();
    // delay(2000);
    // displayLcd->setManual();
    // delay(2000);
    float perc = pot->getPercentage();
    Serial.println(perc);
    servo->setValveTo(perc);
    displayLcd->setValveTo(perc);
    delay(2000);
}
