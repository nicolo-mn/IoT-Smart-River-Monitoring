#include "Potentiometer.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
} 

float Potentiometer::getPercentage(){
  return analogRead(pin) / 1023.0 * 100.0;
}
