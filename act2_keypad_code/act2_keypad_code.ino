#include <Keypad.h>

// passcode
const char PASSCODE[] = "159357";
const int PASSCODE_LENGTH = (sizeof(PASSCODE) / sizeof(PASSCODE[0])) - 1;

// delays
const unsigned long CORRECT_PASSCODE_DELAY = 3000;
const unsigned long INCORRECT_PASSCODE_DELAY = 300;

// led light pins
const int RED_LED = 12;
const int GREEN_LED = 13;

// define keyboard dimensions
const byte ROWS = 3;
const byte COLS = 4;

// keypad characters layout
const char KEY_LAYOUT[ROWS][COLS] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' },
};

// row and column pins
const byte ROW_PINS[ROWS] = { 6, 7, 8 };
const byte COL_PINS[COLS] = { 2, 3, 4, 5 };

// initialize a Keypad object
Keypad keypad = Keypad(makeKeymap(KEY_LAYOUT), ROW_PINS, COL_PINS, ROWS, COLS);

// variables essential for input checking
char currentInput[PASSCODE_LENGTH];
int currentInputLength = 0;



/*  RickRollCode

    AUTHOR: Rowan Packard
    rowanpackard@gmail.com

    DISCLAIMER: The song "Never Gonna Give You Up" by Rick Astley
    is not the creative property of the author. This code simply
    plays a Piezo buzzer rendition of the song.
*/

int piezo = 10;
int player = 9;
int a;  // part index
int b;  // song index
int c;  // lyric index

#define a3f 208  // 208 Hz
#define b3f 233  // 233 Hz
#define b3 247   // 247 Hz
#define c4 261   // 261 Hz MIDDLE C
#define c4s 277  // 277 Hz
#define e4f 311  // 311 Hz
#define f4 349   // 349 Hz
#define a4f 415  // 415 Hz
#define b4f 466  // 466 Hz
#define b4 493   // 493 Hz
#define c5 523   // 523 Hz
#define c5s 554  // 554 Hz
#define e5f 622  // 622 Hz
#define f5 698   // 698 Hz
#define f5s 740  // 740 Hz
#define a5f 831  // 831 Hz

#define rest -1

volatile int beatlength = 100;  // determines tempo
float beatseparationconstant = 0.3;

int threshold;

// Parts 1 and 2 (Intro)

int song1_intro_melody[] = { c5s, e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f };

int song1_intro_rhythmn[] = { 6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10 };

// Parts 3 or 5 (Verse 1)

int song1_verse1_melody[] = { rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
                              rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
                              rest, b3f, b3f, c4, c4s, b3f, c4s, e4f, rest, c4, b3f, b3f, a3f,
                              rest, b3f, b3f, c4, c4s, a3f, a3f, e4f, e4f, e4f, f4, e4f,
                              c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
                              rest, b3f, c4, c4s, a3f, rest, e4f, f4, e4f };

int song1_verse1_rhythmn[] = { 2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
                               1, 1, 1, 1, 3, 1, 2, 1, 5,
                               1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
                               1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
                               5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                               2, 1, 1, 1, 3, 1, 1, 1, 3 };

// Parts 4 or 6 (Chorus)

int song1_chorus_melody[] = { b4f, b4f, a4f, a4f,
                              f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
                              c5s, c5s, c5s, c5s,
                              c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
                              b4f, b4f, a4f, a4f,
                              f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
                              c5s, c5s, c5s, c5s,
                              c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest };

int song1_chorus_rhythmn[] = { 1, 1, 1, 1,
                               3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
                               1, 1, 1, 1,
                               3, 3, 3, 1, 2, 2, 2, 4, 8,
                               1, 1, 1, 1,
                               3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
                               1, 1, 1, 1,
                               3, 3, 3, 1, 2, 2, 2, 4, 8, 4 };


void setup() {
  // set pin modes on both LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // intially light up red LED
  digitalWrite(RED_LED, HIGH);

  // rick roll setup
  pinMode(piezo, OUTPUT);
  pinMode(player, INPUT_PULLUP);

  a = 4;
  b = 0;
  c = 0;

  Serial.begin(9600);
}

void loop() {
  // read the key
  char currentKey = keypad.getKey();

  if (currentKey) {
    // insert the current key registered
    currentInput[currentInputLength] = currentKey;
    // increase the current input length
    ++currentInputLength;

    // DEGBUGGING
    Serial.println("==============================");

    Serial.print("Key Pressed : ");
    Serial.println(currentKey);

    Serial.print("length: ");
    Serial.println(currentInputLength);

    Serial.print("current input: ");
    for (int i = 0; i < currentInputLength; ++i) {
      Serial.print(currentInput[i]);
    }
    Serial.println();
    // DEGBUGGING
  }

  // check passcode if input length
  // is equal to the define length
  // of the passcode
  if (currentInputLength >= PASSCODE_LENGTH) {
    bool passcodeMatched = true;
    for (int i = 0; i < PASSCODE_LENGTH; ++i) {
      if (PASSCODE[i] != currentInput[i]) {
        passcodeMatched = false;
        break;
      }
    }

    if (passcodeMatched) {
      Serial.println("Passcode correct");
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);

      // rickroll the user
      if (digitalRead(player) == LOW) {
        delay(500);
        play();
      } else {
        delay(CORRECT_PASSCODE_DELAY);
      }

      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    } else {
      Serial.println("Passcode incorrect");
      digitalWrite(RED_LED, LOW);
      delay(INCORRECT_PASSCODE_DELAY);
      digitalWrite(RED_LED, HIGH);
    }

    // reset variables
    memset(currentInput, 0, currentInputLength);
    currentInputLength = 0;
  }

  // delay(300);
}


// rick roll
void play() {
  while (true) {
    int notelength;
    if (a == 1 || a == 2) {
      // intro
      notelength = beatlength * song1_intro_rhythmn[b];
      if (song1_intro_melody[b] > 0) {
        tone(piezo, song1_intro_melody[b], notelength);
      }
      b++;
      if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
        a++;
        b = 0;
        c = 0;
      }
    } else if (a == 3 || a == 5) {
      // verse
      notelength = beatlength * 2 * song1_verse1_rhythmn[b];
      if (song1_verse1_melody[b] > 0) {
        tone(piezo, song1_verse1_melody[b], notelength);
        c++;
      }
      b++;
      if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
        a++;
        b = 0;
        c = 0;
      }
    } else if (a == 4 || a == 6) {
      // chorus
      notelength = beatlength * song1_chorus_rhythmn[b];
      if (song1_chorus_melody[b] > 0) {
        tone(piezo, song1_chorus_melody[b], notelength);
        c++;
      }
      b++;
      if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
        a++;
        b = 0;
        c = 0;
      }
    }
    delay(notelength);
    noTone(piezo);
    delay(notelength * beatseparationconstant);

    // end the happiness if pin 9 is not shorted;
    // reset song progression and break loop;
    if (digitalRead(player) == HIGH || a == 7) {
      a = 4;
      b = 0;
      c = 0;
      break;
    }
  }
}
