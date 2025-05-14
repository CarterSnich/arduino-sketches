
// contants
#define BUZZ_PIN 11
#define ACC_PIN 7

// notes
const float C4sharpM = 277.18;
const float E4M = 329.63;
const float F4sharpM = 369.99;
const float G4sharpM = 415.30;
const float B4M = 493.88;
const float C5sharpM = 554.37;
const float E5M = 659.26;

void setup() {
  pinMode(ACC_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
}

void loop() {

  if (digitalRead(ACC_PIN) == HIGH) {
    addMelody(G4sharpM, 500, 100);
    addMelody(G4sharpM, 200, 50);
    addMelody(B4M, 200, 50);
    addMelody(C5sharpM, 500, 50);


    addMelody(G4sharpM, 200, 50);
    addMelody(B4M, 200, 50);
    addMelody(C5sharpM, 200, 50);
    addMelody(E5M, 200, 50);


    addMelody(C5sharpM, 200, 50);
    addMelody(B4M, 200, 50);
    addMelody(G4sharpM, 800, 50);


    addMelody(F4sharpM, 500, 100);
    addMelody(F4sharpM, 200, 50);
    addMelody(G4sharpM, 200, 50);
    addMelody(B4M, 500, 100);


    addMelody(F4sharpM, 200, 50);
    addMelody(E4M, 200, 50);
    addMelody(F4sharpM, 200, 50);
    addMelody(G4sharpM, 200, 50);


    addMelody(F4sharpM, 200, 50);
    addMelody(E4M, 200, 50);
    addMelody(C4sharpM, 1000, 0);
  }
}

void addMelody(float note, int duration, int delayAfter) {
  tone(BUZZ_PIN, note);
  delay(duration);

  noTone(BUZZ_PIN);
  if (delayAfter > 0) delay(delayAfter);
}
