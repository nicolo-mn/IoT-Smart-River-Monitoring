#ifndef __POT__
#define __POT__

class Potentiometer {

private:
  int pin;
 
public: 
  Potentiometer(int pin); 
  float getPercentage();
};

#endif
