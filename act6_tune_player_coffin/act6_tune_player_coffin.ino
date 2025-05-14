int dacPins[] = {2, 4, 7, 8};
int sin16[] = {7, 8, 10, 11, 12, 13, 14, 14, 15, 14, 14, 13, 12, 11,
               10, 8, 7, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6
              };

int lowToneDurations[] = {127, 112, 104, 92, 80, 75, 65};
//                         A    B    C   D   E   F   G
int highToneDurations[] = {56, 49, 45, 39, 33, 30, 25};
//                          a   b   c   d   e   f   g
//char* song = "A"; // for tuning

// coffin dance
char* song = "C C C C E E E E D D D D G G G G a a a a a a a a aaaa E D C B AAA E DD CC BB B C DD C B AAA c b c b c AAA c b c b c AAA E DD CC BB B C DD C B AAA c b c b c AAA c b c b c AAA";

// butterfly
//char* song = "EE E G aa E G a c a G EEEE DD D E GG D C D E D C AA";

// summertime
//char* song = "G G E a G E D C DD D C DD C DD DDDD C a G E D C DD D C DD E DD CCCC aaa G E D C DD D C DD C DD EEE DD CC DDD EEEE";


void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(dacPins[i], OUTPUT);
  }
}

void loop() {
  int i = 0;
  char ch = song[0];
  while (ch != 0) {
    if (ch == ' ') {
      delay(75);
    }
    else if (ch >= 'A' and ch <= 'G') {
      playNote(lowToneDurations[ch - 'A']);
    }
    else if (ch >= 'a' and ch <= 'g') {
      playNote(highToneDurations[ch - 'a']);
    }
    i++;
    ch = song[i];
  }
  delay(5000);
}

void setOutput(byte value) {
  digitalWrite(dacPins[3], ((value & 8) > 0));
  digitalWrite(dacPins[2], ((value & 4) > 0));
  digitalWrite(dacPins[1], ((value & 2) > 0));
  digitalWrite(dacPins[0], ((value & 1) > 0));
}

void playNote(int pitchDelay) {
  long numCycles = 5000 / pitchDelay + (pitchDelay / 4);
  for (int c = 0; c < numCycles; c++) {
    for (int i = 0; i < 32; i++) {
      setOutput(sin16[i]);
      delayMicroseconds(pitchDelay);
    }
  }
}
