#include <SoftwareSerial.h>

SoftwareSerial espSerial(5, 6);

#define CUP_WARN_LEVEL 370

#define RELAY_PIN 8

#define CUP_SENSOR_PWR 9
#define CUP_SENSOR_PIN A0

#define RESERVOIR_SENSOR_PWR 10
#define RESERVOIR_SENSOR_PIN A1


void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(CUP_SENSOR_PWR, OUTPUT);
  pinMode(RESERVOIR_SENSOR_PWR, OUTPUT);

  digitalWrite(CUP_SENSOR_PWR, LOW);
  digitalWrite(RESERVOIR_SENSOR_PWR, LOW);
  
  espSerial.begin(115200);
  delay(100);
}

void loop() {
  int cup_level = readSensor(CUP_SENSOR_PWR, CUP_SENSOR_PIN);
  int reservoir_level = readSensor(RESERVOIR_SENSOR_PWR, RESERVOIR_SENSOR_PIN);

  espSerial.println(reservoir_level);

  if (cup_level <= CUP_WARN_LEVEL) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}

//This is a function used to get the reading
int readSensor(int power_pin, int analog_pin) {
  digitalWrite(power_pin, HIGH);
  delay(10);
  int val = analogRead(analog_pin);
  digitalWrite(power_pin, LOW);

  return val;
}
