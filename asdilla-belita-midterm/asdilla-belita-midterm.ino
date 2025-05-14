#include <LiquidCrystal_I2C.h>

const int btn1 = 2;
const int btn3 = 3;
const int btn4 = 4;
const int btnPlus = 5;
const int btnMinus = 6;
const int btnEquals = 7;
const int btnClear = 8;

LiquidCrystal_I2C lcd(0x27, 20, 4);

String firstNumber = "";
String secondNumber = "";
String operation = "";
String output;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(btnPlus, INPUT_PULLUP);
  pinMode(btnMinus, INPUT_PULLUP);
  pinMode(btnEquals, INPUT_PULLUP);
  pinMode(btnClear, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(btn1) == LOW) {
    if (!operation.length()) {
      firstNumber = firstNumber + "1";
    } else {
      secondNumber = secondNumber + "1";
    }
  } else if (digitalRead(btn3) == LOW) {
    if (!operation.length()) {
      firstNumber = firstNumber + "3";
    } else {
      secondNumber = secondNumber + "3";
    }
  } else if (digitalRead(btn4) == LOW) {
    if (!operation.length()) {
      firstNumber = firstNumber + "4";
    } else {
      secondNumber = secondNumber + "4";
    }
  } else if (digitalRead(btnPlus) == LOW) {
    if (firstNumber.length() && !secondNumber.length()) {
      operation = "+";
    }
  } else if (digitalRead(btnMinus) == LOW) {
    if (firstNumber.length() && !secondNumber.length()) {
      operation = "-";
    }
  } else if (digitalRead(btnEquals) == LOW && secondNumber.length()) {
    int n1 = firstNumber.toInt();
    int n2 = secondNumber.toInt();
    if (operation == "+") {
      output = String(n1 + n2);
    } else {
      output = String(n1 - n2);
    }
  } else if (digitalRead(btnClear) == LOW) {
    firstNumber = "";
    secondNumber = "";
    operation = "";
    output = "";
  }

  lcd.clear();
  lcd.print(firstNumber);
  lcd.print(operation);
  lcd.print(secondNumber);

  if (output.length()) {
    lcd.print("=");
    lcd.print(output);
    while (digitalRead(btnClear) == HIGH) {}
    firstNumber = "";
    secondNumber = "";
    operation = "";
    output = "";
  }

  delay(300);
}
