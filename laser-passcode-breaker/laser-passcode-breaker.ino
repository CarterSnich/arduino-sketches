#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Liquid I2C pin config
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int laser_sensor = 9;
const int buzzer = 10;


// define keyboard dimensions
const byte ROWS = 4;
const byte COLS = 3;

// keypad characters layout
const char KEY_LAYOUT[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '\0', '0', '\0' }
};

// row and column pins
const byte ROW_PINS[ROWS] = { 8, 7, 6, 5 };
const byte COL_PINS[COLS] = { 4, 3, 2 };

// initialize a Keypad object
Keypad keypad = Keypad(makeKeymap(KEY_LAYOUT), ROW_PINS, COL_PINS, ROWS, COLS);

const char passcode[] = "2749";
const int passcode_length = (sizeof(passcode) / sizeof(passcode[0])) - 1;
char passcode_input[passcode_length + 1];
int passcode_input_length = 0;

unsigned long previousMillis = 0;
const long interval = 100;
bool buzzerState = false;

void setup() {
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  pinMode(laser_sensor, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  Serial.print("Passcode: ");
  Serial.println(passcode);
  Serial.print("Length: ");
  Serial.println(passcode_length);

  lcd.setCursor(0, 0);
  lcd.println("    All Clear   ");
}


void loop() {
  if (digitalRead(laser_sensor) == LOW) {

    lcd.setCursor(0, 0);
    lcd.println("Enter passcode: ");
    lcd.setCursor(0, 1);
    lcd.println("                ");

    while (true) {
      unsigned long currentMillis = millis();

      if ((currentMillis - previousMillis) >= interval) {
        previousMillis = currentMillis;
        buzzerState = !buzzerState;
        digitalWrite(buzzer, buzzerState ? HIGH : LOW);
      }

      if (passcode_input_length < passcode_length) {
        char current_key = keypad.getKey();
        if (current_key) {
          Serial.println(current_key);
          passcode_input[passcode_input_length] = current_key;
          passcode_input_length++;

          lcd.setCursor(0, 1);
          lcd.print("      ");
          lcd.print(passcode_input);
          lcd.setCursor(10, 1);
          lcd.print("      ");
        }
      } else {
        passcode_input[4] = '\0';

        if (strcmp(passcode_input, passcode) == 0) {
          digitalWrite(buzzer, LOW);

          passcode_input[0] = ' ';
          passcode_input[1] = ' ';
          passcode_input[2] = ' ';
          passcode_input[3] = ' ';
          passcode_input_length = 0;

          lcd.setCursor(0, 0);
          lcd.println("Passcode Matched");

          delay(3000);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.println("    All Clear   ");

          break;
        }

        lcd.setCursor(0, 0);
        lcd.println(" Wrong Passcode ");

        unsigned int delayStartMillis = millis();
        previousMillis = millis();

        while (true) {
          currentMillis = millis();

          if ((currentMillis - previousMillis) >= interval) {
            previousMillis = currentMillis;
            buzzerState = !buzzerState;
            digitalWrite(buzzer, buzzerState ? HIGH : LOW);
          }

          if ((currentMillis - delayStartMillis) >= 3000) {
            break;
          }
        }


        passcode_input[0] = ' ';
        passcode_input[1] = ' ';
        passcode_input[2] = ' ';
        passcode_input[3] = ' ';
        passcode_input_length = 0;


        lcd.setCursor(0, 0);
        lcd.println("Enter passcode: ");
        lcd.setCursor(0, 1);
        lcd.println("                ");
      }
    }
  }
}
