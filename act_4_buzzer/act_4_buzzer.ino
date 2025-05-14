#define BUZZER 11

void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  unsigned long note = 523.25;
  for (int i = 0; i < 10; i++) {
    tone(BUZZER, note);  // C5 note
    note += 10;
    delay(500);
    noTone(BUZZER);
    delay(500);
  }
}
