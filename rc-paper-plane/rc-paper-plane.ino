#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// constants
const String ssid = "UDP";
const String passphrase = "nullnull";

// UDP
WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];          // buffer for incoming packets

// motor pin config
const int forwardPin = 2;   // D3/GPIO0
const int backwardPin = 0;  // D4/GPIO2
const int accel = 14;       // D5/GPIO14 PWM
Servo servoMot;

void setup() {
  pinMode(forwardPin, OUTPUT);
  pinMode(backwardPin, OUTPUT);
  pinMode(accel, INPUT);
  servoMot.attach(12);

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
  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    String data = incomingPacket;
    String dir = data.substring(0, 2);
    int val = data.substring(2, len).toInt();

    if (dir == "FB") {
      if (val > 0) {
        digitalWrite(forwardPin, HIGH);
        digitalWrite(backwardPin, LOW);
        analogWrite(accel, val);
      } else if (val < 0) {
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, HIGH);
        analogWrite(accel, val * -1);
      } else {
        analogWrite(accel, val);
      }
    } else if (dir == "LR") {
      servoMot.write(val);
    }
    Serial.printf("%s : %d\n", dir, val);
  }
}