#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// RFID
#define UID_KEY "33 F4 C2 14"
#define RST_PIN 9  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// IR Sensors
#define IR_EXIT 2
#define IR_PARK_1 3
#define IR_PARK_2 4

bool park1 = false;
bool park2 = false;

// Servo
#define SERVO 5
Servo servo;



void setup() {
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(IR_EXIT, INPUT_PULLUP);
  pinMode(IR_PARK_1, INPUT_PULLUP);
  pinMode(IR_PARK_2, INPUT_PULLUP);

  servo.attach(SERVO);
  servo.write(180);

  Serial.begin(9600);
}

void loop() {

  if (rfid()) {
    Serial.println("Accepted");
    servo.write(0);
    delay(5000);
    servo.write(180);
  }

  if (digitalRead(IR_EXIT) == LOW) {
    servo.write(0);
    delay(5000);
    servo.write(180);
  }

  park1 = !digitalRead(IR_PARK_1);
  park2 = !digitalRead(IR_PARK_2);


  int spaces = 2;

  if (park1) spaces--;
  if (park2) spaces--;


  Serial.print("Spaces: ");
  Serial.println(spaces);
}

bool rfid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  Serial.println("detected ID");

  String content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  Serial.println(content);

  return content.substring(1) == UID_KEY;
}