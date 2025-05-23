
// Constants
const int ledPin = 13;

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(100);
}
