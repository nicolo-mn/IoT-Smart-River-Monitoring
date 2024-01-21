/**
 * Implemented by 
 * Livia Cardaccia, Nicolò Monaldini,
 * Giovanni Prete, Matteo Sartini
*/

#include <Arduino.h>
#include <Button.h>
#include <LCD.h>
#include <ServoMotor.h>
#include <Potentiometer.h>
#include <Scheduler.h>
#include <ManageValveTask.h>
#include <config.h>


FlagManager *flagManager;
LCD *displayLcd;
Potentiometer *pot;
Scheduler *sched;
ServoMotor *servo;

void setup()
{
    Serial.begin(9600);
    sched = new Scheduler(SCHEDULER_PERIOD);
    Button *btn = new Button(BTN_PIN);
    Serial.println("Sono qui 1");
    displayLcd = new LCD();
    servo = new ServoMotor(SERVO_PIN);
    pot = new Potentiometer(POT_PIN);
    Serial.println("Sono qui 2");
    ManageValveTask *task1 = new ManageValveTask(btn, pot, servo, displayLcd);
    task1->init(MANAGE_PERIOD);
    sched->addTask(task1);
}

void loop()
{
    sched->schedule();
    Serial.println("loop");
    // displayLcd->setManual();
    // delay(1000);
    // displayLcd->setAutomatic();
    // delay(1000);
    // displayLcd->setValveTo(50);
    // delay(1000);
    // displayLcd->setValveTo(100);
    // delay(1000);
    // displayLcd->setValveTo(0);
    // delay(1000);
}
