#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// constants
const String ssid = "UDP";
const String passphrase = "nullnull";

// UDP
WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, passphrase);

  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Udp.begin(localUdpPort);
  Serial.print("UDP port: ");
  Serial.println(localUdpPort);
}


void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
  }
}
