#include "Potentiometer.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
} 

void Potentiometer::sync(){
  value = analogRead(pin);
}

float Potentiometer::getPercentage(){
  return value / 1023.0 * 100.0;
}
