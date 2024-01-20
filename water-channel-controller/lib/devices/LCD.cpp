#include "LCD.h"
#include "ProgessBarIcons.h"

#define FIRST_MSG_LINE 1
#define SECOND_MSG_LINE 2
#define PROGRESS_BAR_LINE 2
#define PERCENTAGE_POS 9

LCD::LCD()
{
    displayLcd = new LiquidCrystal_I2C(0x27, 20, 4);
    displayLcd->init();

    displayLcd->createChar(0, zero);
    displayLcd->createChar(1, one);
    displayLcd->createChar(2, two);
    displayLcd->createChar(3, three);
    displayLcd->createChar(4, four);
    displayLcd->createChar(5, five);

    displayLcd->backlight();
    displayLcd->noDisplay();
}

void LCD::updateProgressBar(unsigned long count)
{
    const int totalCount = 100;
    double factor = totalCount / 100.0; // Uses 100 as the progress bar width

    // Computes completion percentage
    int percent = (count + 1) / factor;

    // Computes the number of full blocks to display
    int number = percent / 5;

    // Computes the partial block to display
    int remainder = percent % 5;

    if (number > 0)
    {
        for (int i = 0; i < number; i++)
        {
            displayLcd->setCursor(i, PROGRESS_BAR_LINE);
            displayLcd->write(5); // Personalized character for full block
        }
    }

    displayLcd->setCursor(number, PROGRESS_BAR_LINE);
    displayLcd->write(remainder);
    displayLcd->setCursor(PERCENTAGE_POS, FIRST_MSG_LINE);
    displayLcd->print(String(count) + "%");
}

void LCD::turnDisplayOn()
{
    displayLcd->display();
    displayLcd->backlight();
    displayLcd->clear();
}

void LCD::turnDisplayOff()
{
    displayLcd->noDisplay();
    displayLcd->noBacklight();
}

void LCD::showOneLineMessage(String message)
{
    displayLcd->clear();
    displayLcd->setCursor(0, FIRST_MSG_LINE);
    displayLcd->print(message);
}

void LCD::showTwoLinesMessage(String firstLine, String secondLine)
{
    showOneLineMessage(firstLine);
    displayLcd->setCursor(0, SECOND_MSG_LINE);
    displayLcd->print(secondLine);
}

void LCD::showWelcomeMessage()
{
    showOneLineMessage("Welcome");
}

void LCD::showEnteringMessage()
{
    showTwoLinesMessage("Proceed to the", "washing area");
}

void LCD::showReadyToWashMessage()
{
    showOneLineMessage("Ready to wash");
}

void LCD::showWashingMessage()
{
    showOneLineMessage("Washing:");
}

void LCD::showWashingCompleteMessage()
{
    showTwoLinesMessage("Washing complete", "you can leave");
}

void LCD::showErrorMessage()
{
    showTwoLinesMessage("Detected a problem", "please wait");
}
