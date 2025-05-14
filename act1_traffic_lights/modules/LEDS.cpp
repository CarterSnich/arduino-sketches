//Leds Sequence Sets Class
//Created by J.D.Rivera
class leds {
public:
  int RED = 2;
public:
  int AMBER = 3;
public:
  int GREEN = 4;
public:
  int SequenceIndex = 0;
public:
  void init() {
    pinMode(RED, OUTPUT);
    pinMode(AMBER, OUTPUT);
    pinMode(GREEN, OUTPUT);
  };
  //Toggle LED Pins;
private:
  void ledArray() {
    int sequences[][2] = { { 0, RED }, { RED, AMBER }, { 0, GREEN }, { 0, AMBER } };
    resetLeds();
    for (int pin = 0; pin <= 2; pin++) {
      digitalWrite(sequences[SequenceIndex][pin], HIGH);
    }
    return;
  }
  //Automatically Progress through the LED Sequences
public:
  void setLeds() {
    ledArray();
    SequenceIndex++;
    if (SequenceIndex > 3) {
      SequenceIndex = 0;
    }
    return;
  };
  //Turn off all LEDS;
public:
  void resetLeds() {
    digitalWrite(RED, LOW);
    digitalWrite(AMBER, LOW);
    digitalWrite(GREEN, LOW);
  }
};