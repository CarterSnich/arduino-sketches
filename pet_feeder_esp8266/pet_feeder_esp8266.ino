
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>


const String ssid = "NodeMCU_ESP8266";

AsyncWebServer server(80);
Servo servo;

#define SERVO_PIN 14
#define BUZZ_PIN 12

String water_level;


void setup() {
  Serial.begin(115200);

  pinMode(BUZZ_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(0);

  if (!LittleFS.begin()) return;

  WiFi.softAP(ssid);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/web/index.html", String());
  });

  server.on("/feed", HTTP_GET, [](AsyncWebServerRequest* request) {
    digitalWrite(BUZZ_PIN, HIGH);
    servo.write(180);
    delay(500);
    servo.write(0);
    digitalWrite(BUZZ_PIN, LOW);
    request->send(200);
  });

  server.on("/water-level", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println(water_level);
    request->send(200, "text/string", water_level);
  });

  server.begin();
}

void loop() {
}
