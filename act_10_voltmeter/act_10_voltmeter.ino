#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// liquid crystal setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// constants
const int poten = A0;

// variable
int value;
float voltage;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.print("Voltage value:");

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  value = analogRead(poten);
  voltage = value * (5.0 / 1023.0);

  char buffer[40];
  dtostrf(voltage, 0, 2, buffer);
  sprintf(buffer, "%sV", buffer);
  
  lcd.setCursor(4, 1);
  lcd.print(buffer);
  Serial.println(buffer);
}
