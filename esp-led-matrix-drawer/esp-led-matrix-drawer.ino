#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

// AP SSID and password
const String ssid = "NodeMCU_ESP8266";
const String password = "nullnull";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Serial connection between the UNO
SoftwareSerial unoSerial(3, 1);

void setup() {
  Serial.begin(115200);

  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // setup Access point
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/index.html", String());
  });

  // handle /send_input request
  server.on("/set-picture", HTTP_POST, [](AsyncWebServerRequest* request) {
//    Serial.print("Receiving input: ");
    if (request->hasParam("MATRIX", true)) {
      AsyncWebParameter* p = request->getParam("MATRIX", true);
      Serial.print(p->value().c_str());
    } else {
      Serial.println("Invalid parameter");
    }
    request->send(200, "text/plain", String());
  });

  // Start server
  server.begin();
}


void loop() {}
