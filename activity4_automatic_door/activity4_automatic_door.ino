#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define UID_KEY "D3 15 8E 19"
#define RST_PIN 9  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

#define SERVO_PIN 2
#define BUZZER_PIN 3

#define DOOR_OPEN_DURATION 10000

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo servo;  // Servo intialization

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PLACE ID CARD");

  servo.attach(SERVO_PIN);
  servo.write(0);

  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("NO NEW CARD");
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Show UID on serial monitor
  String content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    // Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
 0

  if (content.substring(1) == UID_KEY) {
    servo.write(180);
    Serial.print("ACCESS GRANTED");

    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("ACCESS GRANTED");

    tone(BUZZER_PIN, 1000);
    delay(DOOR_OPEN_DURATION);
    noTone(BUZZER_PIN);

    servo.write(0);
    Serial.println("DOOR CLOSED");

    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("DOOR CLOSED");

  } else {
    Serial.println("UNAUTHORIZED ACCESS");

    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("UNAUTHORIZED");
    lcd.setCursor(0, 1);
    lcd.print("ACCESS");

    tone(BUZZER_PIN, 2000);
    delay(100);
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    tone(BUZZER_PIN, 2000);
    delay(100);
    noTone(BUZZER_PIN);

    delay(3000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PLACE ID CARD");
}
