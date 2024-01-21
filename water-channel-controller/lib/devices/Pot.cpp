#include "Pot.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
} 

float Potentiometer::getValue(){
  return analogRead(pin) / 1023.0;
}
