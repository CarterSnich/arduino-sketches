#include <IRremote.hpp>
//#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// constants
const String ssid = "smart-hotel";
const String passphrase = "12345678";
String type;

// UDP
WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];          // buffer for incoming packets

//Light pins

int leds_pins[4] = { 16, 5, 4, 0 };  //
int led_pins_status[4] = { 0, 0, 0, 0 };
int pin_count;


//Remote Ip data
IPAddress recieved_ip;
uint16_t recieved_port;
IPAddress IP;

//IR related
#define DISABLE_CODE_FOR_RECEIVER // Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
IRsend sender;
int SEND_PIN = 14;





void setup() {
  Serial.begin(9600);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, passphrase);


  IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Udp.begin(localUdpPort);
  Serial.printf("UDP port: %d\n", localUdpPort);


  //set all led_pins to Output mode
  pin_count = sizeof(leds_pins) / sizeof(int);
  for (int pin = 0; pin < pin_count; pin++) {
    pinMode(leds_pins[pin], OUTPUT);
  }


  IrSender.begin(SEND_PIN);

}


void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);

    recieved_ip = Udp.remoteIP();
    recieved_port = Udp.remotePort();

    if (len > 0) incomingPacket[len] = 0;
    String data = incomingPacket;
    String dir = data.substring(0, 2);
    int val = data.substring(2, len).toInt();

    if (data == "isConnected") {
      String jsonData = "{'type':'connection','value':'true'}";
      replyDevice(jsonData);
    }

    if (data == "discover") {
      Serial.println("IP add: " + IP.toString());
      String jsonData = "{'type':'address','value':'" + IP.toString() + "'}";
      replyDevice(jsonData);
    }
    //send to parse command
    parseCommand(data);
  }
}







//remote and fixtures command parser
void parseCommand(String data) {

  String index;
  String state;


  type = getValue(data, '|', 0);

  if (type == "remote") {
      
      
      sendIR(data);

  } else if (type == "light") {

    index = getValue(data, '|', 1);
    state = getValue(data, '|', 2);

    if (state == "toggle") {
      toggleLeds(index.toInt());
    }


  } else if (type == "misc") {
    index = getValue(data, '|', 1);
    if (index == "curtain") {
    }
  }

  return;
}





char packetBuffer[50];
char replyBuffer[50];
String strCon;

//Reply to connecting device:
void replyDevice(String message) {

  message.toCharArray(replyBuffer, 50);
  Udp.beginPacket(recieved_ip, recieved_port);
  Udp.write(replyBuffer);
  Udp.endPacket();
  return;
}


//Toggling Lights
void toggleLeds(int index) {


  if (led_pins_status[index] == 0) {
    digitalWrite(leds_pins[index], HIGH);
    led_pins_status[index] = 1;
  } else {
    digitalWrite(leds_pins[index], LOW);
    led_pins_status[index] = 0;
  }
  return;
}


//helper function for sending IR data using IR emmiter
void sendIR(String data){

  String str = getValue(data, '|', 1);
  int str_len = str.length() + 1;
  char stra[10];
  str.toCharArray(stra, 10);
  char* hex = stra; 

    char *ptr;

    uint32_t datav = strtoul(hex, &ptr, 16);
    uint8_t len = 32;


    Serial.println(hex);
    Serial.println(datav);    

    //sender.sendNEC(datav, len);
    IrSender.sendNECRaw(datav,0);



 

  //IrSender.sendNECRaw((uint32_t)command, 0);
  

}



//String Functions

//Split and get the String at Sepcific Location of index
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}