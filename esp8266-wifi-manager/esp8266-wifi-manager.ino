#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const String ssid = "esp8266";
const String password = "nullnull";

AsyncWebServer server(80);

JsonDocument statusUpdate;
int scanResult = NULL;

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin()) {
    Serial.println("Failed to intialize LittleFS.");
    return;
  };

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  WiFi.softAP(ssid, password);

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String());
  });

  server.on("/vue.global.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/vue.global.js", String());
  });

  server.on("/fetch-updates", HTTP_GET, [](AsyncWebServerRequest *request) {
    WiFi.scanNetworksAsync(
      [](int r) {
        scanResult = r;

        if (r > 0) {
          String ssid;
          int32_t rssi;
          uint8_t encryptionType;
          uint8_t *bssid;
          int32_t channel;
          bool hidden;
          int scanResult;

          statusUpdate["is_connected"] = WiFi.isConnected();

          JsonArray networks = statusUpdate["networks"].to<JsonArray>();

          for (int8_t i = 0; i < r; i++) {
            JsonObject wifi = networks.add<JsonObject>();

            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

            wifi["ssid"] = ssid;
            wifi["is_encrypted"] = encryptionType != ENC_TYPE_NONE;
            wifi["rssi"] = rssi;
            wifi["bssid"] = "";
            wifi["channel"] = channel;
            wifi["is_hidden"] = hidden;
          }
        }

        scanResult = r;
      },
      true);

    while (scanResult == NULL) {}

    if (scanResult > 0) {
      String body;
      serializeJson(statusUpdate, body);
      request->send(200, "text/json", body);
    } else if (scanResult == 0) {
      request->send(200, "text/json", "{}");
    } else {
      request->send(500, "text/plain", "Network scan error.");
    }

    scanResult = NULL;
    statusUpdate.clear();
  });

  server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (!WiFi.disconnect()) {
      request->send(500, "text/plain", "Failed to disconnect from current network.");
    }

    AsyncWebParameter *p_ssid = request->getParam("ssid", true);
    AsyncWebParameter *p_is_encrypted = request->getParam("encrypted", true);
    int isConnected = 0;

    if (p_is_encrypted->value().toInt()) {
      AsyncWebParameter *p_password = request->getParam("password", true);
      isConnected = WiFi.begin(
        p_ssid->value().c_str(),
        p_password->value().c_str(),
        0,
        NULL,
        true);
    } else {
      isConnected = WiFi.begin(
        p_ssid->value().c_str(), 
        emptyString, 
        0, 
        NULL, 
        true);
    }

    if (isConnected) {
      request->send(200, "text/plain", "Connected to a network.");
    } else {
      request->send(500, "text/plain", "Failed to connect a network.");
    }
  });

  server.on("/disconnect", HTTP_POST, [](AsyncWebServerRequest *request) {
    WiFi.disconnect();

    unsigned long last = millis();
    while (WiFi.isConnected() && millis() - last >= 3000) {}

    if (!WiFi.isConnected()) {
      request->send(200, "text/plain", "Disconnected from any network.");
    } else {
      request->send(500, "text/plain", "Failed to disconnect from network.");
    }
  });

  server.begin();
}


void loop() {}
