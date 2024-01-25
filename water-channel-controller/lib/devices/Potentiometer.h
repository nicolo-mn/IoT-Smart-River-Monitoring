#ifndef __POT__
#define __POT__

class Potentiometer {

private:
  int pin;
  int percentageValue;
  bool hasChangedFlag;
 
public: 
  Potentiometer(int pin);
  void sync();
  int getPercentage();
  bool hasChanged();
};

#endif
