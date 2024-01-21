#ifndef __POT__
#define __POT__

class Potentiometer {

private:
  int pin;
  float value;
 
public: 
  Potentiometer(int pin);
  void sync();
  float getPercentage();
};

#endif
