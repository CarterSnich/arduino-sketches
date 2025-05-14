#include <Servo.h>
#define SERVO_PIN 3
#define MAX_LEFT 115
#define MAX_RIGHT 50
#define ZERO 80

Servo servo;

void setup() {
  servo.attach(SERVO_PIN);
  servo.write(ZERO);
  Serial.begin(9600);
}

void loop() {
  while (!Serial.available()) {}

  String input = Serial.readStringUntil('\n');
  if (input) {
    int val = input.toInt();
    Serial.println(val);

    if (val >= MAX_LEFT) {
      servo.write(MAX_LEFT);
    } else if (val <= MAX_RIGHT) {
      servo.write(MAX_RIGHT);
    } else {
      servo.write(val);
    }
  }
}
