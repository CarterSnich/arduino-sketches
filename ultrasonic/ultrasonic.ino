#include <NewPing.h>

const int distance = 70;
const int relay_pin = 8;

const int trig_pin = 10;
const int echo_pin = 11;
NewPing sonar(trig_pin, echo_pin, 200);  // sensor function

void setup() {
  pinMode(relay_pin, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  Serial.println(sonar.ping_cm());
  
  if (sonar.ping_cm() <= distance) {
    digitalWrite(relay_pin, !digitalRead(relay_pin));
  }

  delay(1000);
}
