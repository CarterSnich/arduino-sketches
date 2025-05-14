#include <LiquidCrystal_I2C.h>

#define duration 10
#define btnPin 2
#define buzzPin 3

/* 
 0 = waiting for start
 1 = in count down
 2 = countdown finished
 */
int state = 0;
unsigned long startTime;
unsigned long elapsedTime;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Press to Start");

  pinMode(btnPin, INPUT);
  pinMode(buzzPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (state == 0 && digitalRead(btnPin) == HIGH) {
    state = 1;
    startTime = millis();
    lcd.clear();
  }

  if (state == 1) {
    elapsedTime = (millis() - startTime) / 1000;

    if (elapsedTime >= duration) {
      state = 2;
      digitalWrite(buzzPin, HIGH);
    }

    lcd.setCursor(0, 0);
    lcd.print(String(10 - elapsedTime) + " ");
  }

  if (state == 2) {
    if (digitalRead(btnPin) == HIGH) {
      state = 0;
      digitalWrite(buzzPin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Press to Start");
      delay(500);
    }
  }
}
