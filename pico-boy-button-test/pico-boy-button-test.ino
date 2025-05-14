
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin configuration
#define KEYPAD_A 28
#define KEYPAD_B 27
#define KEYPAD_UP 29
#define KEYPAD_RIGHT 26
#define KEYPAD_DOWN 15
#define KEYPAD_LEFT 14
#define SPEAKER_PIN 9

void setup() {
  pinMode(KEYPAD_A, INPUT_PULLUP);
  pinMode(KEYPAD_B, INPUT_PULLUP);
  pinMode(KEYPAD_UP, INPUT_PULLUP);
  pinMode(KEYPAD_LEFT, INPUT_PULLUP);
  pinMode(KEYPAD_DOWN, INPUT_PULLUP);
  pinMode(KEYPAD_RIGHT, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);              // Start at top-left corner
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (digitalRead(KEYPAD_A) == LOW) {
    display.println("KEYPAD_A: True");
  } else {
    display.println("KEYPAD_A: False");
  }

  if (digitalRead(KEYPAD_B) == LOW) {
    display.println("KEYPAD_B: True");
  } else {
    display.println("KEYPAD_B: False");
  }

  if (digitalRead(KEYPAD_UP) == LOW) {
    display.println("KEYPAD_UP: True");
  } else {
    display.println("KEYPAD_UP: False");
  }

  if (digitalRead(KEYPAD_RIGHT) == LOW) {
    display.println("KEYPAD_RIGHT: True");
  } else {
    display.println("KEYPAD_RIGHT: False");
  }

  if (digitalRead(KEYPAD_DOWN) == LOW) {
    display.println("KEYPAD_DOWN: True");
  } else {
    display.println("KEYPAD_DOWN: False");
  }

  if (digitalRead(KEYPAD_LEFT) == LOW) {
    display.println("KEYPAD_LEFT: True");
  } else {
    display.println("KEYPAD_LEFT: False");
  }

  display.display();  // Show initial text
}
