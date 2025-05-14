int ledNum = 8;
int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int ledDelay;
int flowDirection = 1;
int currentLed = 0;
unsigned long newtime;

void setup() {
  for (int x = 0; x < ledNum; x++) {
    pinMode(ledPins[x], OUTPUT);
  }
  newtime = millis();
}

void loop() {
  ledDelay = analogRead(A0);

  if ((millis() - newtime) > ledDelay) {
    for (int x = 0; x < ledNum; x++) {
      digitalWrite(ledPins[x], LOW);
    }

    digitalWrite(ledPins[currentLed], HIGH);
    currentLed += flowDirection;

    if (currentLed == ledNum - 1) flowDirection = -1;
    if (currentLed == 0) flowDirection = 1;

    newtime = millis();
  }
}
