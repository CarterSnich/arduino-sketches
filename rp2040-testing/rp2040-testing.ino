#define LED 14
#define TOUCH 15

int lastTouchState;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(TOUCH, INPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  // int currentTouchState = digitalRead(TOUCH);
  // if (currentTouchState != lastTouchState) {
  //   digitalWrite(LED, digitalRead(LED) == HIGH);    
  // }

  // lastTouchState = currentTouchState;
}
