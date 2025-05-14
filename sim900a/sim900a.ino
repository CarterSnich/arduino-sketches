/*THIS TUTORIAL USED GSM SIM900A MINI V3.9.2
 
  Connect 5VT to D9 and 5VR to D10
  Feed GSM SIM900A with Arduino's 5V

  Code by IDAYU SABRI - MYBOTIC
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
char msg;
char call;

void setup()
{
  mySerial.begin(115200);   // Setting the baud rate of GSM Module  
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println();
  delay(100);
}

void loop()
{  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
  }
 if (myiSerial.available()iSerial.write(mySerial.read());
}

void SendMessage()
{
  mySe#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// constants
const String ssid = "UDP";
const String passphrase = "nullnull";

// UDP
WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

// motor pin config
const int forwardPin = 2; // D3/GPIO0
const int backwardPin = 0; // D4/GPIO2
const int accel = 14; // D5/GPIO14 PWM
Servo servoMot; // D6/GPIO12 PWM

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
  if (packetSize)
  {
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
    }
    else if (dir == "LR") {
      servoMot.write(val);
    }
    Serial.printf("%s : %d\n", dir, val);
  }

}
rial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+639456070958\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available()>0)
  {
    msg=mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD+60XXXXXXXXX;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call=mySerial.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}


