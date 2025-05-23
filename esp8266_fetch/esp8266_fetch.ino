
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

/* Set these to your desired credentials. */
const String ssid = "NodeMCU_ESP8266";
const String password = "nullnull";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  // put your setup code here, to run once:
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/index.html", String());
  });

  // Route to load style.css file
  server.on("/web/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/web/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/web/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/web/script.js", "text/js");
  });

  // handle /send_input request
  server.on("/send_input", HTTP_POST, [](AsyncWebServerRequest * request) {
    Serial.println("Receiving input: ");
    if (request->hasParam("KEY_DIRECTION", true)) {
      AsyncWebParameter* p = request->getParam("KEY_DIRECTION", true);
      Serial.println(p->value().c_str());
    }
    request->send(200, "text/plain", String());
  });

  // Start server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
