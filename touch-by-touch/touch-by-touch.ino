#define TOUCH 6
#define LED 7

int lastTouchState = LOW;
int currentTouchState = LOW;

void setup() {
  pinMode(TOUCH, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  currentTouchState = digitalRead(TOUCH);

  if (currentTouchState == HIGH && currentTouchState != lastTouchState) {
    digitalWrite(LED, digitalRead(LED) == LOW ? HIGH : LOW);
  }

  lastTouchState = currentTouchState;
}
