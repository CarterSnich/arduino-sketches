
// contants
const int buzzPin = 7;
const int ledPin = 5;

// notes
const float G3major = 196.0;
const float A3major = 220.0;
const float A3sharpM = 233.08;
const float E3major = 164.81;
const float F3major = 174.61;
const float D3major = 146.83; 

void setup () {
    pinMode(buzzPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
}

void loop() {
    addMelody(G3major, 50, 0);

    addMelody(A3major, 200, 100);
    
    addMelody(A3major, 100, 100);
    
    addMelody(A3major, 200, 100);
    
    addMelody(G3major, 100, 100);

    addMelody(G3major, 500, 500);

    
    addMelody(A3major, 50, 0);

    addMelody(A3sharpM, 200, 100);
    
    addMelody(A3sharpM, 100, 100);
    
    addMelody(A3sharpM, 200, 100);
    
    addMelody(A3major, 100, 100);

    addMelody(A3major, 500, 500);


    addMelody(G3major, 50, 0);

    addMelody(A3major, 200, 100);
    
    addMelody(A3major, 100, 100);
    
    addMelody(A3major, 200, 100);
    
    addMelody(G3major, 100, 100);

    addMelody(G3major, 500, 500);

    
    addMelody(E3major, 50, 0);

    addMelody(F3major, 200, 100);
    
    addMelody(F3major, 100, 100);
    
    addMelody(F3major, 200, 100);
    
    addMelody(D3major, 100, 100);

    addMelody(D3major, 500, 500);


    delay(3000);
}
/*
 * float note The frequency passed to `tone()` function.
 * int duration Duration of the tone.
 * int delayAfter Duration of mute after the tone.
 */
void addMelody(float note, int duration, int delayAfter) {
    digitalWrite(ledPin, HIGH);
    tone(buzzPin, note);
    delay(duration);

    digitalWrite(ledPin, LOW);
    noTone(buzzPin);
    if (delayAfter > 0) delay(delayAfter);
}
