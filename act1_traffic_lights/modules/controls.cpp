
//Control Button Key Listener
//Created by J.D.Rivera
class controlPanel {
  //Input Pins
  //int buttonUp = A1;
  //int buttonDown = A0;
  int modeButton = 13;
  unsigned long prevButtonMs = 0;
public:
  unsigned long currentMs = 0;
  //public: int currentMode = 0;
public:
  bool isLongPress;  //Detected Long Press
  //Detect rapid Press
public:
  bool rapidPress;  //Detecting Rapid Presses
  bool upWake = false;
  //Prev Button States
  int prevState = 0;
  int currState = 0;
  int longPressInterval = 500;
  int clickInterval = 1000;
  int buttonValue;
  unsigned long msLongPress;
  //Initilize the pinmodes for the Pins used:
public:
  void init() {
    pinMode(modeButton, INPUT);
  }
  //Listen to key Press
public:
  void onKeyPress() {
    currentMs = millis();
    //int buttonValueUp = analogRead(buttonUp);
    //int buttonValueDown = analogRead(buttonDown);
    int buttonMode = digitalRead(modeButton);
    if (buttonMode == HIGH) {
      currState = 1;
    } else {
      currState = 0;
    }
    if (currentMs - prevButtonMs >= 100) {
      if (prevState != currState && currState > 0) {
        buttonResponder();  //Send To Helper Function
        prevButtonMs = currentMs;
        //Detect Long Press
      } else if (currentMs - prevButtonMs >= longPressInterval && (prevState == currState && currState > 0)) {
        isLongPress = true;
        //Reset States
      } else if (prevState != currState && currState <= 0) {
        prevButtonMs = currentMs;
        isLongPress = false;
      }
      prevState = currState;
    }
    return;
  }
  //Returns a bool if an activity from buttons occur recently.
public:
  bool isPressed() {
    if (upWake) {
      upWake = false;
      return true;
    }
    return false;
  }
  unsigned long prevClickMs;  //Click interval ms
public:
  void buttonResponder() {
    //Detect Rapid presses and Ignore it
    if (millis() - prevClickMs < clickInterval) {
      prevClickMs = millis();
      rapidPress = true;
      return;
    }
    prevClickMs = millis();
    if (currState == 1) {
      rapidPress = false;
      upWake = true;
    }
    return;
  };
};