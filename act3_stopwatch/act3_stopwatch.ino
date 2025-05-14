#include "SevSegShift.h"

// define data pins on the shift register
#define SHIFT_PIN_DS 9
#define SHIFT_PIN_STCP 8
#define SHIFT_PIN_SHCP 7

/* 
  Instantiate a seven segment controller object with:
  - segment pins controlled via 1 shift register and 
  - digit pins connected to the Arduino directly
 */
SevSegShift sevsegshift(
  SHIFT_PIN_DS,
  SHIFT_PIN_SHCP,
  SHIFT_PIN_STCP,
  1,    // number of shift registers
  true  // true if Digits are connected to Arduino directly
);

/*
  6 - Reset timer
  5 - Start/Pause
  4 - Lap
  3 - Scroll up
  2 - Scroll down
 */
const int btnCount = 4;
const int btnPins[btnCount] = { 6, 5, 4, 3 };
int btnLastStates[btnCount] = { LOW, LOW, LOW, LOW };
unsigned long btnInitialPressTime = 0;
unsigned long lastTimerChange = 0;

// variables for controlling the stopwatch
unsigned long currentTime = 0;
unsigned long startTime = 0;
unsigned long pauseStartTime = 0;
unsigned long timer = 0;
bool isReverse = false;
bool isRunning = false;  // stays true if `isPaused` is true
bool isPaused = false;

// buzzer
int buzzerPin = 2;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = { 13, 12, 11, 10 };            // These are the PINS of the ** Arduino **
  byte segmentPins[] = { 0, 1, 2, 3, 4, 5, 6, 7 };  // these are the PINs of the ** Shift register **
  bool resistorsOnSegments = true;                  // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE;             // See README.md for options
  bool updateWithDelays = false;                    // Default 'false' is Recommended
  bool leadingZeros = true;                         // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false;                     // Use 'true' if your decimal point doesn't exist or isn't connected

  sevsegshift.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
                    updateWithDelays, leadingZeros, disableDecPoint);
  sevsegshift.setBrightness(90);

  for (int i = 0; i < btnCount; ++i) {
    pinMode(btnPins[i], INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  // set current button states
  int btnCurrentStates[btnCount];
  for (int i = 0; i < btnCount; ++i) {
    btnCurrentStates[i] = digitalRead(btnPins[i]);
  }

  // reset button press event
  if (btnCurrentStates[0] == HIGH && btnLastStates[0] == LOW && isRunning && isPaused && !isReverse) {
    resetVariables();
  }

  // start/pause button press event
  if (btnCurrentStates[1] == HIGH && btnLastStates[1] == LOW) {
    if (!isRunning) {
      startTime = millis();
      isRunning = true;
      isPaused = false;
    } else if (!isPaused && !isReverse) {
      pauseStartTime = millis();
      isPaused = true;
    } else if (isPaused && !isReverse) {
      startTime = startTime + (millis() - pauseStartTime);
      pauseStartTime = 0;
      isPaused = false;
    }
  }

  // button events when the stopwatch
  // is not running and not paused
  if (!isRunning && !isPaused) {
    // timer increment and decrement
    for (int i = 2; i <= 3; i++) {
      if (btnCurrentStates[i] == HIGH && btnLastStates[i] == LOW) {
        updateCurrentTime(i == 2 ? 1000 : -1000);
      } else if (btnCurrentStates[i] == HIGH) {
        if (btnInitialPressTime == 0) {
          btnInitialPressTime = millis();
        } else {
          if (millis() - btnInitialPressTime >= 500) {
            if (lastTimerChange == 0) lastTimerChange = millis();

            if (millis() - lastTimerChange >= 300) {
              updateCurrentTime(i == 2 ? 5000 : -5000);
              lastTimerChange = millis();
            }
          }
        }
      } else if (btnCurrentStates[i] == LOW && btnLastStates[i] == HIGH) {
        btnInitialPressTime = 0;
        lastTimerChange = 0;
      }
    }
  }


  // remember curret button states
  for (int i = 0; i < btnCount; ++i) {
    btnLastStates[i] = btnCurrentStates[i];
  }

  if (isRunning) {
    if (!isReverse) {
      // counting only progresses when
      // it is not paused
      if (!isPaused) {
        currentTime = millis() - startTime;
      }

      // when paused, display is blank
      // if seconds is divisible by two
      // else, the display shows the time
      if (isPaused && (millis() / 1000) % 2) {
        sevsegshift.blank();
      } else {
        printTime();
      }
    } else {
      currentTime = (timer + 1000) - (millis() - startTime);
      if (currentTime <= 0) {
        while (digitalRead(btnPins[1]) == LOW) {
          if ((millis() / 1000) % 2) {
            sevsegshift.blank();
            noTone(buzzerPin);
          } else {
            printTime();
            tone(buzzerPin, 512);
          }
          sevsegshift.refreshDisplay();
        }
        noTone(buzzerPin);
        btnLastStates[1] = HIGH;
        resetVariables();
      } else {
        printTime();
      }
    }
  } else {
    // calculate minutes and seconds
    printTime();
  }

  // Must run repeatedly to avoid
  // catastrohpic events from happening
  sevsegshift.refreshDisplay();
}

void updateCurrentTime(int delta) {
  if (delta > 0) {
    currentTime += delta;
  } else if (delta < 0) {
    currentTime = currentTime <= abs(delta) ? 0 : currentTime + delta;
  }
  if (currentTime == 0) {
    isReverse = false;
    timer = 0;
  } else {
    isReverse = true;
    timer = currentTime;
  }
}

void printTime() {
  if (currentTime == 0) {
    sevsegshift.setNumber(0000, 2);
  } else {
    int minutes = currentTime / 60000;
    int seconds = (currentTime % 60000) / 1000;
    sevsegshift.setNumber(minutes * 100 + seconds, 2);
  }
}

void resetVariables() {
  currentTime = 0;
  startTime = 0;
  pauseStartTime = 0;
  timer = 0;
  isReverse = false;
  isRunning = false;
  isPaused = false;
}