
// constants
const int RELAY_PIN = 2;
const int SIGNAL_PIN = 3;

void setup () {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);
}

void loop() {
  if (digitalRead(SIGNAL_PIN) == HIGH) {
    digitalWrite(RELAY_PIN, LOW);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }
}
