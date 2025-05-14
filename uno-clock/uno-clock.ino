#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

const char* DAYS_OF_WEEK[7] = {
  "Sun", "Mon", "Tue",
  "Wed", "Thu", "Fri", "Sat"
};

const char* MONTHS_OF_YEAR[12] = {
  "Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug",
  "Sep", "Oct", "Nov", "Dec"
};

LiquidCrystal_I2C lcd(0x27, 16, 2);

              // IO/DAT, CLK, CE/RST
ThreeWire myWire(8, 9, 10);
RtcDS1302<ThreeWire> Rtc(myWire);
RtcDateTime rtcOld;
const int compileTimeOffset = 0;

char mDate[17];
char mTime[9];

const int ALARM_BUTTON = 12;
const int ALARM_BUZZER = 13;

bool isRinging = false;
unsigned int alarmHour = 6;
unsigned int alarmMinute = 0;

// const int RGB_RED = 6;
// const int RGB_GREEN = 5;
// const int RGB_BLUE = 3;
// const int colorIntervalDuration = 1000;
// int currentPin = 0;

unsigned long last = millis();

void setup() {
  lcd.init();
  lcd.backlight();

  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) + compileTimeOffset;

  // Update with compiled time if RTC is invalid
  if (!Rtc.IsDateTimeValid()) {
    Rtc.SetDateTime(compiled);
  }

  // Disable RTC write-protection
  if (Rtc.GetIsWriteProtected()) {
    Rtc.SetIsWriteProtected(false);
  }

  // Run RTC
  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }

  // Update RTC if time if not set
  if (Rtc.GetDateTime() <= compiled) {
    Rtc.SetDateTime(compiled);
  }

  pinMode(ALARM_BUTTON, INPUT_PULLUP);
  pinMode(ALARM_BUZZER, OUTPUT);

  // pinMode(RGB_RED, OUTPUT);
  // pinMode(RGB_GREEN, OUTPUT);
  // pinMode(RGB_BLUE, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  RtcDateTime rtcNow = Rtc.GetDateTime();
  Serial.println(rtcNow);

  /* 
    Funcitionalities 
    Uncomment to load a functionality.
  */

  /* core functionality */
  rtcHandler(rtcNow);

  /* alarm, reads from variable
   BUTTON is used to turn off alarm */
  alarmHandler(now, rtcNow);

  /* allow setting alarm with BUTTON */
  // setAlarm(); 

  /* RGB lights
   I suggest using automated RGB light,
   to lessen processing usage */
  // rgbHandler(now);
}

void rtcHandler(RtcDateTime& rtcNow) {
  if (rtcOld < rtcNow) {
    parseDate(rtcNow);
    parseTime(rtcNow);

    lcdPrint(mDate, 0, 0);
    lcdPrint(mTime, 4, 1);
  }

  rtcOld = rtcNow;
}

void parseDate(const RtcDateTime& dt) {
  snprintf_P(
    mDate,
    17,
    PSTR("%s, %02u %s %04u"),
    DAYS_OF_WEEK[dt.DayOfWeek()],
    dt.Day(),
    MONTHS_OF_YEAR[dt.Month() - 1],
    dt.Year());
}

void parseTime(const RtcDateTime& dt) {
  snprintf_P(
    mTime,
    9,
    PSTR("%02u:%02u:%02u"),
    dt.Hour(),
    dt.Minute(),
    dt.Second());
}

void lcdPrint(char str[], int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(str);
}

void alarmHandler(unsigned long now, RtcDateTime& dt) {
  int alarmButtonState = digitalRead(ALARM_BUTTON);

  if (alarmButtonState == HIGH && dt.Hour() == alarmHour && dt.Minute() == alarmMinute && dt.Second() == 0) {
    isRinging = true;
  }

  if (alarmButtonState == LOW && isRinging) {
    isRinging = false;
    lcd.backlight();
    digitalWrite(ALARM_BUZZER, LOW);
    return;
  }

  if (isRinging) {
    if ((now / 1000) % 2 == 0) {
      lcd.backlight();
      digitalWrite(ALARM_BUZZER, HIGH);
    } else {
      lcd.noBacklight();
      digitalWrite(ALARM_BUZZER, LOW);
    }
  }
}

void setAlarm() {
  int alarmButtonState = digitalRead(ALARM_BUTTON);

  if (alarmButtonState == LOW && !isRinging) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Set Alarm");

    char alarm[6];

    const unsigned long alarmSetTimeOut = 3000;
    const unsigned long btnHoldTimeOut = 200;
    unsigned long now;
    unsigned long last = millis();

    do {
      now = millis();

      snprintf_P(alarm, 6, PSTR("%02u:%02u"), alarmHour, alarmMinute);
      lcd.setCursor(6, 1);
      lcd.print(alarm);

      if (digitalRead(ALARM_BUTTON) == LOW && (now - last) >= btnHoldTimeOut) {
        alarmHour = alarmMinute >= 55 ? alarmHour + 1 : alarmHour;
        alarmHour = alarmHour > 23 ? 0 : alarmHour;
        alarmMinute = alarmMinute >= 55 ? 0 : alarmMinute + 5;
        last = now;
      }

    } while (now - last <= alarmSetTimeOut);
  }
}

// void rgbHandler(unsigned long now) {
//   unsigned int duration = now - last;
//   int colorIntensity = map(duration, 0, colorIntervalDuration, 0, 255);

//   switch (currentPin) {
//     // RED
//     case 0:
//       setColor(colorIntensity, 0, 255 - colorIntensity);
//       break;

//     // GREEN
//     case 1:
//       setColor(255 - colorIntensity, colorIntensity, 0);
//       break;

//     // BLUE
//     case 2:
//       setColor(0, 255 - colorIntensity, colorIntensity);
//       break;
//   }

//   if (duration >= colorIntervalDuration) {
//     last = now;
//     currentPin = currentPin >= 2 ? 0 : currentPin + 1;
//   }
// }

// void setColor(int r, int g, int b) {
//   analogWrite(RGB_RED, r);
//   analogWrite(RGB_GREEN, g);
//   analogWrite(RGB_BLUE, b);
// }