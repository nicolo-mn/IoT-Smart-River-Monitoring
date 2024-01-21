#include "Button.h"
#include <Arduino.h>

Button::Button(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
    pressed = false;
    clicked = false;
}

void Button::sync()
{
    bool wasPressed = pressed;
    pressed = digitalRead(pin) == HIGH;
    if (!pressed)
    {
        if (wasPressed)
        {
            clicked = true;
        }
        else
        {
            if (clicked)
            {
                clicked = false;
            }
        }
    }
    else if (pressed)
    {
        clicked = false;
    }
}

bool Button::isClicked()
{
    return clicked;
}