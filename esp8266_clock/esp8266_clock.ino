// LiquidCrystal I2C libraries
#include <LiquidCrystal_I2C.h>

// libraries required for RTC
#include <ThreeWire.h>
#include <RtcDS1302.h>

// ESP8266 wifi and UDP libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// clock logo
#include "clock_logo.h"

// Liquid I2C pin config
LiquidCrystal_I2C lcd(0x27, 16, 2);

//               D7, D6, D5
ThreeWire myWire(12, 13, 14);  // IO/DATA, SCLK/CLK, CE/RESET
RtcDS1302<ThreeWire> Rtc(myWire);

// clock configurations
bool enable24hour = false;
bool showSeconds = false;
int dateFormat = 0;

// months
const String MONTHS[12] = {
  "JAN",
  "FEB",
  "MAR",
  "APR",
  "MAY",
  "JUN",
  "JUL",
  "AUG",
  "SEP",
  "OCT",
  "NOV",
  "DEC"
};

// WAP credentials
const char* ssid = "esp8266_clock";  //Replace with your own SSID
const char* password = "12345678";   //Replace with your own password

// Wifi UDP setup
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[256];

void setup() {
  Serial.begin(115200);

  // init LCD
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // init RTC
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

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
    Serial.println("RTC is older than compile time! (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  //  init WAP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // initialize UDP broadcast
  Udp.begin(localUdpPort);
  Serial.print("UDP port: ");
  Serial.println(localUdpPort);


  //  lcd.setCursor(0, 0);
  //  lcd.print("Time: ");
  //  lcd.setCursor(0, 1);
  //  lcd.print("Date: ");
  lcd.createChar(0, topLeft);
  lcd.createChar(1, topRight);

  lcd.setCursor(0, 0);
  lcd.write(byte(0)); // top left
  lcd.setCursor(1, 0);
  lcd.write(byte(1)); // top right
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.println(incomingPacket);

    // grab configuration, if possible
    String data = incomingPacket;
    String conf = data.substring(0, 6);

    // turn on 12 hour format
    if (conf == "ON24HR") {
      enable24hour = true;
    }

    // turn off 12 hour format
    else if (conf == "OF24HR") {
      enable24hour = false;
    }

    // show seconds
    else if (conf == "SHOSEC") {
      showSeconds = true;
    }

    // hide seconds
    else if (conf == "HIDSEC") {
      showSeconds = false;
    }

    // date format 0
    else if (conf == "DTFMT0") {
      dateFormat = 0;
    }

    // date format 1
    else if (conf == "DTFMT1") {
      dateFormat = 1;
    }

    // date format 2
    else if (conf == "DTFMT2") {
      dateFormat = 2;
    }

    // store new datetime
    else {
      uint32_t secsSince2000 = strtoul(incomingPacket, NULL, 10);
      RtcDateTime syncedTime = RtcDateTime(secsSince2000);
      Rtc.SetDateTime(syncedTime);
    }
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }
  
  printDateTime(now);


  // Print IP addresses of connected clients
  struct station_info* stationList = wifi_softap_get_station_info();
  struct station_info* station = stationList;

  while (station != NULL) {
    // send back RTC datetime
    Udp.beginPacket(station->ip.addr, 5003);
    Udp.write(String(Rtc.GetDateTime().TotalSeconds()).c_str());
    Udp.endPacket();
    station = STAILQ_NEXT(station, next);
  }

  // Free memory
  wifi_softap_free_station_info();

  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  int mTimeLen = 9;
  int mDateLen = 11;

  if (enable24hour) mTimeLen = 6;
  if (showSeconds) mTimeLen = mTimeLen + 3;

  if (dateFormat == 2) {
    mDateLen = mDateLen + ((dt.Day() >= 10) + 1);
  }

  char mTime[mTimeLen];
  char mDate[mDateLen];

  if (!enable24hour) {
    int hour = dt.Hour();
    char amPm[] = "AM";

    if (dt.Hour() == 0) {
      hour = 12;
      amPm[0] = 'P';
    }
    else if (dt.Hour() > 12) {
      hour = dt.Hour() - 12;
      amPm[0] = 'P';
    }

    if (showSeconds) {
      snprintf_P(
        mTime,
        countof(mTime),
        PSTR("%02u:%02u:%02u %s"),
        hour,
        dt.Minute(),
        dt.Second(),
        amPm
      );
    }
    else {
      snprintf_P(
        mTime,
        countof(mTime),
        PSTR("%02u%s%02u %s"),
        hour,
        (dt.Second() % 2) == 0 ? ":" : " ",
        dt.Minute(),
        amPm
      );
    }
  }

  else {
    if (showSeconds) {
      snprintf_P(
        mTime,
        countof(mTime),
        PSTR("%02u:%02u:%02u"),
        dt.Hour(),
        dt.Minute(),
        dt.Second()
      );
    } else {
      snprintf_P(
        mTime,
        countof(mTime),
        PSTR("%02u%s%02u"),
        dt.Hour(),
        (dt.Second() % 2) == 0 ? ":" : " ",
        dt.Minute()
      );
    }
  }

  switch (dateFormat) {
    // MM/DD/YYYY
    case 0:
      snprintf_P(
        mDate,
        countof(mDate),
        PSTR("%02u/%02u/%04u"),
        dt.Month(),
        dt.Day(),
        dt.Year()
      );
      break;

    // DD/MM/YYYY
    case 1:
      snprintf_P(
        mDate,
        countof(mDate),
        PSTR("%02u/%02u/%04u"),
        dt.Day(),
        dt.Month(),
        dt.Year()
      );
      break;

    // MMM D, YYYY
    case 2:
      snprintf_P(
        mDate,
        countof(mDate),
        PSTR("%s %u, %u"),
        MONTHS[dt.Month() - 1],
        dt.Day(),
        dt.Year()
      );
      break;
  }

  // TODO: lcd print time and date
  String padding = " ";
  for (int i = 0; i < (14 - mTimeLen); i++) {
    padding.concat(" ");
  }
  lcd.setCursor(2, 0);
  lcd.print(padding);

  padding = " ";
  for (int i = 0; i < (14 - mDateLen); i++) {
    padding.concat(" ");
  }
  lcd.setCursor(2, 1);
  lcd.print(padding);

  lcd.setCursor(17 - mTimeLen, 0);
  lcd.print(mTime);
  lcd.setCursor(17 - mDateLen, 1);
  lcd.print(mDate);
}
