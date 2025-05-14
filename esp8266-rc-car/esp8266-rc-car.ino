#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define MAX_LEFT 145
#define ZERO 83
#define MAX_RIGHT 28

#define SERVO_PIN 5            // D1 - GPIO5
#define MOTOR_FORWARD 4        // D2 - GPIO4
#define MOTOR_REVERSE 13       // D7 - GPIO13
#define HEADLIGHTS_PIN 12      // D6 - GPIO12
#define REVERSE_LIGHTS_PIN 14  // D5 - GPIO14
#define BUZZER_PIN 16

Servo servo;

WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];          // buffer for incoming packets

void setup() {
  servo.attach(SERVO_PIN);
  servo.write(ZERO);

  pinMode(MOTOR_FORWARD, OUTPUT);
  pinMode(MOTOR_REVERSE, OUTPUT);
  pinMode(HEADLIGHTS_PIN, OUTPUT);
  pinMode(REVERSE_LIGHTS_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.softAP("awto-awto");
  Udp.begin(localUdpPort);

  Serial.begin(115200);
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
      digitalWrite(MOTOR_FORWARD, LOW);
      digitalWrite(MOTOR_REVERSE, LOW);
    } else if (dir == "BR") {
      if (val) {
        digitalWrite(REVERSE_LIGHTS_PIN, HIGH);
        digitalWrite(MOTOR_FORWARD, HIGH);
        digitalWrite(MOTOR_REVERSE, HIGH);
      } else {
        digitalWrite(REVERSE_LIGHTS_PIN, LOW);
        digitalWrite(MOTOR_FORWARD, LOW);
        digitalWrite(MOTOR_REVERSE, LOW);
      }
    } else if (dir == "FW") {
      digitalWrite(MOTOR_REVERSE, LOW);
      analogWrite(MOTOR_FORWARD, val);
    } else if (dir == "RV") {
      digitalWrite(MOTOR_FORWARD, LOW);
      analogWrite(MOTOR_REVERSE, val);
    } else if (dir == "LR") {
      servo.write(val);
    } else if (dir == "HL") {
      digitalWrite(HEADLIGHTS_PIN, !digitalRead(HEADLIGHTS_PIN));
    } else if (dir == "HO") {
      if (val) {
        digitalWrite(BUZZER_PIN, HIGH);
      } else {
        digitalWrite(BUZZER_PIN, LOW);
      }
    }
  }
}
