#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char myName[] = "Joe Mar Closa";
char yrSect[] = "                           BSIT AI21";
int timeDelay = 400;

void setup()
{
  lcd.init();
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(myName);
  lcd.setCursor(0, 1);
  lcd.print(yrSect);

  for (int i = 0; i < 1; i++) {
    lcd.scrollDisplayLeft();
    delay(timeDelay);
  }
}
