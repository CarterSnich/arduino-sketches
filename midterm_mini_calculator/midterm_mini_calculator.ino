#include <LiquidCrystal_I2C.h>

#define BTN_FOUR 2
#define BTN_FIVE 3
#define BTN_SEVEN 4
#define BTN_PLUS 5
#define BTN_MINUS 6
#define BTN_EQUALS 7
#define BTN_CLR 8

String num1 = "";
String num2 = "";
int op = NULL;
bool cursor = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte bar[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.createChar(0, bar);

  lcd.setCursor(0, 0);
  lcd.print("     KASIO");
  delay(1000);

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(0);
    delay(100);
  }

  pinMode(BTN_FOUR, INPUT);
  pinMode(BTN_FIVE, INPUT);
  pinMode(BTN_SEVEN, INPUT);
  pinMode(BTN_PLUS, INPUT);
  pinMode(BTN_MINUS, INPUT);
  pinMode(BTN_EQUALS, INPUT);
  pinMode(BTN_CLR, INPUT);
}

void loop() {
  if (digitalRead(BTN_FOUR) == HIGH) {

    if (op == 0) {
      num1 = num1 + "4";
    } else {
      num2 = num2 + "4";
    }

  } else if (digitalRead(BTN_FIVE) == HIGH) {

    if (op == 0) {
      num1 = num1 + "5";
    } else {
      num2 = num2 + "5";
    }

  } else if (digitalRead(BTN_SEVEN) == HIGH) {

    if (op == 0) {
      num1 = num1 + "7";
    } else {
      num2 = num2 + "7";
    }

  } else if (digitalRead(BTN_PLUS) == HIGH) {

    if ((op == 0 || op == 1 || op == 2) && num1.length() && !num2.length()) {
      op = 1;
    }

  } else if (digitalRead(BTN_MINUS) == HIGH) {

    if ((op == 0 || op == 1 || op == 2) && num1.length() && !num2.length()) {
      op = 2;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(num1);
  lcd.print(op == 0 ? "" : op == 1 ? "+"
                                   : "-");
  lcd.print(num2);

  if (cursor) lcd.write(0);
  cursor = !cursor;

  if (digitalRead(BTN_EQUALS) == HIGH && num2.length()) {
    int n1 = num1.toInt();
    int n2 = num2.toInt();
    int output;

    if (op == 1) {
      output = n1 + n2;
    } else {
      output = n1 - n2;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(num1);
    lcd.print(op == 0 ? "" : op == 1 ? "+"
                                     : "-");
    lcd.print(num2);
    lcd.print("=");
    lcd.print(output);

    while (digitalRead(BTN_CLR) == LOW) {}
    
  } else if (digitalRead(BTN_CLR) == HIGH) {
    reset();
  }

  delay(200);
}

void reset() {
  num1 = "";
  num2 = "";
  op = 0;
  lcd.clear();
}
