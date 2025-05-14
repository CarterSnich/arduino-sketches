#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char temp[14];
char humid[14];

#define countof(a) (sizeof(a) / sizeof(a[0]))

void setup() {
  dht.begin();

  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("T: ");

  lcd.setCursor(0, 1);
  lcd.print("H: ");

  Serial.begin(115200);
}

void loop() {
  // pass true for fahrenheit
  float newTc = dht.readTemperature();
  float newTf = dht.readTemperature(true);
  lcd.setCursor(3, 0);
  if (isnan(newTc)) {
    sprintf(temp, "%s", "ERROR");
  } else {
    snprintf_P(temp, sizeof(temp), PSTR("%d.%02d%s/%d.%02d%s"), (int)newTc, (int)(newTc * 100) % 100, "C", (int)newTf, (int)(newTf * 100) % 100, "F");
  }
  lcd.print(temp);

  // Read Humidity
  // if humidity read failed, don't change h value
  float newH = dht.readHumidity();
  lcd.setCursor(3, 1);
  if (isnan(newH)) {
    sprintf(humid, "%s", "ERROR");
  } else {
    snprintf_P(humid, sizeof(humid), PSTR("%d.%02d %s"), (int)newH, (int)(newH * 100) % 100, "%RH");
  }
  lcd.print(humid);
}
