#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define SERVO_PIN 5           // D1 - GPIO5
#define MOTOR_FORWARD_PIN 4   // D2 - GPIO4
#define MOTOR_REVERSE_PIN 13  // D7 - GPIO13
#define HEADLIGHTS_PIN 16     // D0 - GPIO16
#define TAIL_LIGHTS_PIN 14    // D5 - GPIO14
#define BUZZER_PIN 12         // D6 - GPIO12

Servo servo;

WiFiUDP Udp;
char incomingPacket[255];

void setup() {
  servo.attach(SERVO_PIN);
  servo.write(83);

  pinMode(MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_REVERSE_PIN, OUTPUT);
  pinMode(HEADLIGHTS_PIN, OUTPUT);
  pinMode(TAIL_LIGHTS_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.softAP("awto-awto");
  Udp.begin(5003);
}

void loop() {
  int packetSize = Udp.parsePacket();

  if (packetSize) {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;

    String data = incomingPacket;
    String dir = data.substring(0, 2);
    int val = data.substring(2, len).toInt();
    Serial.print(dir);
    Serial.println(val);

    if (dir == "CO") {
      // Coasting, motor free spins but no input
      digitalWrite(MOTOR_FORWARD_PIN, LOW);
      digitalWrite(MOTOR_REVERSE_PIN, LOW);
    } else if (dir == "BR") {
      // Brake
      digitalWrite(TAIL_LIGHTS_PIN, val);
      digitalWrite(MOTOR_FORWARD_PIN, val);
      digitalWrite(MOTOR_REVERSE_PIN, val);
    } else if (dir == "FW") {
      // Forward
      digitalWrite(MOTOR_REVERSE_PIN, LOW);
      analogWrite(MOTOR_FORWARD_PIN, val);
    } else if (dir == "RV") {
      // Reverse
      digitalWrite(MOTOR_FORWARD_PIN, LOW);
      analogWrite(MOTOR_REVERSE_PIN, val);
    } else if (dir == "LR") {
      // Sterring
      servo.write(val);
    } else if (dir == "HL") {
      // Headlights
      digitalWrite(HEADLIGHTS_PIN, !digitalRead(HEADLIGHTS_PIN));
    } else if (dir == "HO") {
      // Horn
      digitalWrite(BUZZER_PIN, val);
    }
  }
}
