
#include <NewPing.h> // Ultrasonic
#include <Servo.h> // servo

#include <ESP8266WiFi.h> // ESP8266
#include <WiFiUdp.h> // UDP


// distances
const int sonarMaxDistance = 200; // max distance for the sensor to read
const int dangerDistance = 20; // distance to avoid
const int criticalDistance = 10;

const int echoPin = 14; // D5/GPIO14
const int trigPin = 12; // D6/GPIO12

NewPing sonar(trigPin, echoPin, sonarMaxDistance);



Servo servo;
const int servoPin = 2; // D4/GPIO2
// Servo positions (rough calibrations):
const int fullLeft = 170;
const int frontCenter = 90; // 57 old val
const int fullRight = 10;
const int asd[4] = {fullLeft,  frontCenter,  fullRight, frontCenter};
int view = 0;


// L298N control pins
const int leftMotorForwardPin = 16; // D0/GPIO16
const int leftMotorBackwardPin = 5; // D1/GPIO5
const int rightMotorForwardPin = 4; // D2/GPIO4
const int rightMotorBackwardPin = 0; // D3/GPIO0

// motor speed pins
const int leftMotorSpeedPin = 15;  // D8/GPIO15
const int rightMotorSpeedPin = 13; // D7/GPIO13

// motor speeds
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

// biasing motors speed
int leftMotorSpeedBias = 0;
int rightMotorSpeedBias = 0;

// turning duration
const int turningDuration = 100;


// UDP
WiFiUDP Udp;
unsigned int localUdpPort = 5003;  // local port to listen on
char incomingPacket[255];          // buffer for incoming packets

// set wifi credentials
const String ssid = "Mobot"; // SSID
const String passphrase = "12345678"; // password

// mobot mode
bool controlled = false;


void setup() {
  // attach servo to pin
  servo.attach(servoPin);

  // initially position the sensor in front and center
  servo.write(frontCenter);

  // set pin modes
  pinMode(rightMotorForwardPin, OUTPUT);
  pinMode(rightMotorBackwardPin, OUTPUT);
  pinMode(leftMotorForwardPin, OUTPUT);
  pinMode(leftMotorBackwardPin, OUTPUT);

  // make the robot move forward if initially not controlled
  if (!controlled) {
    motorForward();
    setLeftMotorSpeed(255);
    setRightMotorSpeed(255);
  }

  // initiate wifi
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid, passphrase);

  // print the AP IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // print the UDP port
  Udp.begin(localUdpPort);
  Serial.print("UDP port: ");
  Serial.println(localUdpPort);

  Serial.begin(115200);
}

void loop() {
  int packetSize = Udp.parsePacket();
  String key;
  int val;

  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

    // grab the data
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;
    String data = incomingPacket;

    // parse data
    key = data.substring(0, 2);
    val = data.substring(2, len).toInt();

    // set mobot mode
    if (key == "MD") {
      controlled = val;
      if (controlled) {
        motorStop();
      } else {
        motorForward();
        leftMotorSpeedBias = 0;
        rightMotorSpeedBias = 0;
        setLeftMotorSpeed(255);
        setRightMotorSpeed(255);

      }
    }

    // control the mobot
    if (controlled) {

      // control forward/backward
      if (key == "FB") {
        if (val > 0) {
          motorForward();
          leftMotorSpeed = val;
          rightMotorSpeed = val;
        } else if (val < 0) {
          motorBackward();
          leftMotorSpeed = val * -1;
          rightMotorSpeed = val * -1;
        } else {
          motorStop();
        }
      }


      // control turning
      else if (key == "LR") {
        if (val < 0) {
          rightMotorSpeedBias = val * -1;
        } else if (val > 0) {
          leftMotorSpeedBias = val;
        } else {
          leftMotorSpeedBias = 0;
          rightMotorSpeedBias = 0;
        }
      }
    }

  }

  // control mobot
  if (controlled) {
    setLeftMotorSpeed(leftMotorSpeed);
    setRightMotorSpeed(rightMotorSpeed);
  }

  // mobot mode obstacle
  else {
    servo.write(asd[view]);
    delay(100);
    view = view >= 4 ? 0 : view + 1;

    // detect if the distance is less or equal to
    // danger distance
    if (sonar.ping_cm() <= dangerDistance) {
      motorStop();
      int left = checkAndPingLeft();
      int right = checkAndPingRight();

      int distance = 0;

      // if distance on left is greater
      // than the distance on the right
      if (left > right) {
        servo.write(fullLeft);

        // detect until the distance is greater
        // than the danger distance
        do {
          distance = sonar.ping_cm();

          // move backward if the distance
          // is critical
          if (distance <= criticalDistance) {
            motorBackward();
            delay(100);
            motorStop();
          }

          delay(300);
          motorTurnLeft();
        }
        while (distance <= dangerDistance);
      }

      // if distance on right is greater
      // than the distance on the left
      else {
        servo.write(fullRight);
        
        // detect until the distance is greater
        // than the danger distance
        do {
          distance = sonar.ping_cm();

          // detect until the distance is greater
          // than the danger distance
          if (distance <= criticalDistance) {
            motorBackward();
            delay(100);
            motorStop();
          }

          delay(300);
          motorTurnRight();
        }
        while (distance <= dangerDistance);
      }

      servo.write(frontCenter);
      motorForward();
      setLeftMotorSpeed(255);
      setRightMotorSpeed(255);
    }

  }
}

// ping left distance
int checkAndPingLeft () {
  servo.write(fullLeft);
  delay(300);
  int distance = sonar.ping_cm();

  // move backward if the distance is critical
  if (distance <= criticalDistance) {
    motorBackward();
    delay(100);
    motorStop();
  } else {
    delay(300);
  }

  return distance;
}


// ping right distance
int checkAndPingRight () {
  servo.write(fullRight);
  delay(300);
  int distance = sonar.ping_cm();

  // move backward if the distance is critical
  if (distance <= criticalDistance) {
    motorBackward();
    delay(100);
    motorStop();
  } else {
    delay(300);
  }

  return distance;
}

void setLeftMotorSpeed(int val) {
  analogWrite(leftMotorSpeedPin, val - leftMotorSpeedBias);
}

void setRightMotorSpeed(int val) {
  analogWrite(rightMotorSpeedPin, val - rightMotorSpeedBias);
}

void motorForward() {
  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void motorBackward() {
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, HIGH);
}

void motorStop() {
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, LOW);
}

void motorTurnLeft () {
  digitalWrite(leftMotorForwardPin, HIGH);
  digitalWrite(leftMotorBackwardPin, LOW);
  digitalWrite(rightMotorForwardPin, LOW);
  digitalWrite(rightMotorBackwardPin, HIGH);

  delay(turningDuration);
  motorStop();
}

void motorTurnRight () {
  digitalWrite(leftMotorForwardPin, LOW);
  digitalWrite(leftMotorBackwardPin, HIGH);
  digitalWrite(rightMotorForwardPin, HIGH);
  digitalWrite(rightMotorBackwardPin, LOW);

  delay(turningDuration);
  motorStop();
}
