#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// Liquid I2C pin config
LiquidCrystal_I2C lcd(0x27, 16, 2);

//               D6, D5, D7
ThreeWire myWire(9, 8, 10);  // IO/DAT, SCLK/CLK, CE/RS
RtcDS1302<ThreeWire> Rtc(myWire);

char mTime[12];
char mDate[11];

void setup() {
  // init LCD
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // init RTC
  Rtc.Begin();        
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  Serial.begin(9600);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  Serial.print("Seconds since Jan 1, 2000: ");
  Serial.println(compiled.TotalSeconds());

  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);

  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  parseTime(dt);
  parseDate(dt);

  lcdPrint(mTime, 3, 0);
  lcdPrint(mDate, 3, 1);
}

void parseTime(const RtcDateTime& dt) {
  int hour = dt.Hour() >= 12 ? dt.Hour() - 12 : dt.Hour();
  String amPm = dt.Hour() >= 12 ? "PM" : "AM";
  lcd.setCursor(12, 0);
  lcd.print(amPm);

  snprintf_P(
    mTime,
    countof(mTime),
    PSTR("%02u:%02u:%02u"),
    hour,
    dt.Minute(),
    dt.Second());
}

void parseDate(const RtcDateTime& dt) {
  snprintf_P(
    mDate,
    countof(mDate),
    PSTR("%02u/%02u/%04u"),
    dt.Month(),
    dt.Day(),
    dt.Year());
}

void lcdPrint(char str[], int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(str);
}
