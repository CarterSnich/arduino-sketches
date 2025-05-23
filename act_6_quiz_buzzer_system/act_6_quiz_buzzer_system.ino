
// constants
const int ledPins[3] = {2, 3, 4};
const int btnPins[3] = {9, 10, 11};

const int buzzPin = 7;
const int resetLed = 5;
const int resetBtn = 8;

const float buzzFreq = 536.96;

// variables
bool waiting = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(resetLed, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(resetBtn, INPUT_PULLUP);

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3 && waiting; i++) {
    if (digitalRead(btnPins[i]) == LOW) {
      tone(buzzPin, buzzFreq, 100);
      digitalWrite(ledPins[i], HIGH);
      waiting = false;
      break;
    }
  }

  if (!waiting && digitalRead(resetBtn) == LOW) {
    Serial.println("Reset");
    
    digitalWrite(resetLed, HIGH);
    tone(buzzPin, buzzFreq);
    delay(100);
    digitalWrite(resetLed, LOW);
    noTone(buzzPin);
    for (int i = 0; i < 3; i++) digitalWrite(ledPins[i], LOW);
    waiting = true;
  }
}
