// constants
const int ledSet1[3] = {2, 3, 4};
const int ledSet2[3] = {5, 6, 7};

void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < 3; i++) {
    pinMode(ledSet1[i], OUTPUT);
    pinMode(ledSet2[i], OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  // green - red
  digitalWrite(ledSet1[2], HIGH);
  digitalWrite(ledSet2[0], HIGH);

  delayAndLowAllLed();

  // yellow - red
  digitalWrite(ledSet1[1], HIGH);
  digitalWrite(ledSet2[0], HIGH);

  delayAndLowAllLed();

  // red - green
  digitalWrite(ledSet1[0], HIGH);
  digitalWrite(ledSet2[2], HIGH);

  delayAndLowAllLed();

  // red - yellow
  digitalWrite(ledSet1[0], HIGH);
  digitalWrite(ledSet2[1], HIGH);

  delayAndLowAllLed();

}

void delayAndLowAllLed () {
  delay(2000);
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledSet1[i], LOW);
    digitalWrite(ledSet2[i], LOW);
  }
}
