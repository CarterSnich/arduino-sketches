#include <Keypad.h>

#define PIEZO_PIN 13


float noteFrequencies[20] = {
  220.00,  // A3
  233.08,  // A#3
  246.94,  // B3
  261.63,  // C4
  277.18,  // C#4
  293.66,  // D4
  311.13,  // D#4
  329.63,  // E4
  349.23,  // F4
  369.99,  // F#4
  392.00,  // G4
  415.30,  // G#4
  440.00,  // A4
  466.16,  // A#4
  493.88,  // B4
  523.25,  // C5
  554.37,  // C#5
  587.33,  // D5
  622.25,  // D#5
  659.26   // E5
};

void setup() {
  pinMode(PIEZO_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int key = keypad.getKey();

  if (key) {
    Serial.println(key);
  }
}
