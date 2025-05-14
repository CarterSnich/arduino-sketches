#include <NewPing.h>

// Configurations
const int DETECTION_DISTANCE = 10;  // in inches
const int LOOP_DELAY = 50;         // in milliseconds

// Ultrasonic setup
NewPing sonar(
  /* trigger */ 12,
  /* echo */ 11,
  /* max distance */ 200);

// Relay setup
const int RELAY_PIN = 7;
int relayState = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  delay(LOOP_DELAY);

  int distance = sonar.ping_in();
  Serial.println(distance);

  int currentRelayState = relayState;
  relayState = distance <= DETECTION_DISTANCE ? HIGH : LOW;

  if (currentRelayState != relayState) {
    digitalWrite(RELAY_PIN, relayState);
  }
}
