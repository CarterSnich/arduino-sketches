#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

/* Constructor */
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0,20,"Hello World!");
}

/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {
}
//
//#define SCREEN_WIDTH 128
//#define SCREEN_HEIGHT 64
//
//#define KEYPAD_A 28
//#define KEYPAD_B 27
//#define KEYPAD_UP 29
//#define KEYPAD_RIGHT 26
//#define KEYPAD_DOWN 15
//#define KEYPAD_LEFT 14
//#define SPEAKER_PIN 9
//
//void setup() {
//  pinMode(KEYPAD_A, INPUT_PULLUP);
//  pinMode(KEYPAD_B, INPUT_PULLUP);
//  pinMode(KEYPAD_UP, INPUT_PULLUP);
//  pinMode(KEYPAD_LEFT, INPUT_PULLUP);
//  pinMode(KEYPAD_DOWN, INPUT_PULLUP);
//  pinMode(KEYPAD_RIGHT, INPUT_PULLUP);
//  pinMode(SPEAKER_PIN, OUTPUT);
//}
//
//void loop() {
//  display.clearDisplay();
//  display.setCursor(0, 0);
//
//  if (digitalRead(KEYPAD_A) == LOW) {
//    display.println("KEYPAD_A: True");
//  } else {
//    display.println("KEYPAD_A: False");
//  }
//
//  if (digitalRead(KEYPAD_B) == LOW) {
//    display.println("KEYPAD_B: True");
//  } else {
//    display.println("KEYPAD_B: False");
//  }
//
//  if (digitalRead(KEYPAD_UP) == LOW) {
//    display.println("KEYPAD_UP: True");
//  } else {
//    display.println("KEYPAD_UP: False");
//  }
//
//  if (digitalRead(KEYPAD_RIGHT) == LOW) {
//    display.println("KEYPAD_RIGHT: True");
//  } else {
//    display.println("KEYPAD_RIGHT: False");
//  }
//
//  if (digitalRead(KEYPAD_DOWN) == LOW) {
//    display.println("KEYPAD_DOWN: True");
//  } else {
//    display.println("KEYPAD_DOWN: False");
//  }
//
//  if (digitalRead(KEYPAD_LEFT) == LOW) {
//    display.println("KEYPAD_LEFT: True");
//  } else {
//    display.println("KEYPAD_LEFT: False");
//  }
//}
