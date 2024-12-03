#include "unity.h"
#include "LiquidCrystal.h"
#include <Arduino.h>

const int rs = 2, en = 4, d4 = 5, d5 = 18, d6 = 19, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("hello, world!");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}

void loop()
{
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(millis() / 1000);
}