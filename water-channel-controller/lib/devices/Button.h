#ifndef __BUTTON__
#define __BUTTON__
class Button
{
private:
    int pin;
    bool pressed;
    bool clicked;
public:
    Button(int pin);
    void sync();
    bool isClicked();
};

#endif