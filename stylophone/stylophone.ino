#define PIEZO_PIN 28

const float notes[20] = {
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

int activePin = -1;

void setup() {
  pinMode(PIEZO_PIN, OUTPUT);
  for (int i = 0; i < 20; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  for (int pin = 0; pin < 20; pin++) {
    if (digitalRead(pin) == LOW) {
      if (pin != activePin) {
        Serial.println(pin);
        activePin = pin;
        digitalWrite(LED_BUILTIN, HIGH);
        tone(PIEZO_PIN, (unsigned int)notes[pin]);
      }
      return;
    }
  }

  noTone(PIEZO_PIN);
  activePin = -1;
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("none");
}
