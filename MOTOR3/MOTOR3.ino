#include <NewPing.h>
#include <Servo.h>
#include <AFMotor.h>

#define RIGHT A2
#define LEFT A3
#define TRIGGER_PIN A1
#define ECHO_PIN A0
#define MAX_DISTANCE 100


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor LeftMotor(4, MOTOR12_1KHZ);
AF_DCMotor RightMotor(1, MOTOR12_1KHZ);

Servo servo;

bool following = true;

int pos = 90;
int leftToRight = true;
bool stay = false;

void setup() {
  Serial.begin(9600);
  servo.attach(10);

  LeftMotor.setSpeed(160);
  RightMotor.setSpeed(160);
}


void loop() {
  if (Serial.available()) {
    int data = Serial.read();

    // UNFOLLOW
    if (data == 50) {
      following = false;
    }
  }

  servo.write(pos);

  delay(50);
  unsigned int distance = sonar.ping_in();

  Serial.println(distance);


  if (following) {
    if (distance <= 20 && distance > 0) {
      stay = true;

      if (pos > 90) {
        // LEFT
        leftToRight = true;
        LeftMotor.setSpeed(pos - 90);
        RightMotor.setSpeed(140 - (pos - 90));
        LeftMotor.run(FORWARD);
        RightMotor.run(FORWARD);
      } else if (pos < 90) {
        // RIGHT
        leftToRight = false;
        LeftMotor.setSpeed(140 - pos);
        RightMotor.setSpeed(pos + 20);
        LeftMotor.run(FORWARD);
        RightMotor.run(FORWARD);
      } else {
        LeftMotor.setSpeed(140);
        RightMotor.setSpeed(140);

        LeftMotor.run(FORWARD);
        RightMotor.run(FORWARD);
      }
    } else {
      stay = false;
      LeftMotor.run(RELEASE);
      RightMotor.run(RELEASE);
    }

    if (!stay) {
      if (pos <= 0) {
        leftToRight = false;
      } else if (pos >= 180) {
        leftToRight = true;
      }

      if (leftToRight) {
        pos--;
      } else {
        pos++;
      }
    }
  } else {
    LeftMotor.setSpeed(80);
    RightMotor.setSpeed(80);
    LeftMotor.run(RELEASE);
    RightMotor.run(RELEASE);

    while (!following) {
      if (Serial.available()) {

        int val = Serial.read();
        switch (val) {
          // FOLLOW
          case 49:
            following = true;
            break;

          // FORWARD
          case 51:
            LeftMotor.run(FORWARD);
            RightMotor.run(FORWARD);
            break;

          // REVERSE
          case 52:
            LeftMotor.run(BACKWARD);
            RightMotor.run(BACKWARD);
            break;

          // LEFT
          case 53:
            LeftMotor.run(RELEASE);
            RightMotor.run(FORWARD);
            delay(500);
            LeftMotor.run(FORWARD);
            RightMotor.run(FORWARD);
            break;

          // RIGHT
          case 54:
            LeftMotor.run(FORWARD);
            RightMotor.run(RELEASE);
            delay(500);
            LeftMotor.run(FORWARD);
            RightMotor.run(FORWARD);
            break;

          // STOP
          case 55:
            LeftMotor.run(RELEASE);
            RightMotor.run(RELEASE);
            break;
        }
      }
    }
  }
}