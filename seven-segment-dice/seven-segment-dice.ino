#include "SevSegShift.h"

#define SHIFT_PIN_DS   8 /* Data input PIN */
#define SHIFT_PIN_STCP 7 /* Shift Register Storage PIN */
#define SHIFT_PIN_SHCP 6 /* Shift Register Shift PIN */

//Instantiate a seven segment controller object (with Shift Register functionality)
SevSegShift sevseg(
  SHIFT_PIN_DS,
  SHIFT_PIN_SHCP,
  SHIFT_PIN_STCP,
  /*
     number of shift registers there is only 1 Shiftregister
     used for all Segments (digits are on Controller)
     default value = 2 (see SevSegShift example)
  */
  1,
  /*
     Digits are connected to Arduino directly
     default value = false (see SevSegShift example)
  */
  true
);

const int btn = 10; // button

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {5, 4, 3, 2}; // These are the PINS of the ** Arduino **
  byte segmentPins[] = {0, 2, 4, 6, 7, 1, 3, 5}; // these are the PINs of the ** Shift register **
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(100);

  randomSeed(analogRead(0));

  pinMode(btn, INPUT);

  Serial.begin(9600);
}



void loop() {
  if (digitalRead(btn) == HIGH) {
    int randNum = random(1, 21);

    for (int i = 0; i < 60; i++) {
      int fake = random(0, 20);
      sevseg.setNumber(fake);
      Serial.println(fake);
      sevseg.refreshDisplay();

      if (i >= 50) {
        delay(200);
      sevseg.refreshDisplay();
        continue;
      }
      delay(50);
      sevseg.refreshDisplay();
    }


    if (randNum <= 10) {
      sevseg.setNumber(randNum);
      Serial.println(randNum);
    } else {
      char randChar[] = {' ', ' ', ' ', 54 + randNum, '\0'};
      sevseg.setChars(randChar);
      Serial.print(randNum);
      Serial.print(" : ");
      Serial.println(randChar);
    }

  }

  sevseg.refreshDisplay();
}
