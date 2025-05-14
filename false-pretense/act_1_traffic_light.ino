const int BUTTON_PIN = 13;  // change this to match the pin you have your button connected to

int buttonState = 0;                 // current state of the button
int lastButtonState = 0;             // previous state of the button
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long lastPressTime = 0;     // the last time the button was pressed
bool buttonLocked = false;           // flag to indicate if button is currently locked out

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // set up the button pin as an input with internal pull-up resistor
  Serial.begin(9600);                 // initialize serial communication for debugging
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);  // read the state of the button
  if (reading != lastButtonState) {       // check if the button state has changed
    lastDebounceTime = millis();          // reset the debounce timer
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {  // check if debounce timer has expired
    if (reading != buttonState) {                       // check if the button state has changed again
      buttonState = reading;                            // update button state
      if (buttonState == LOW && !buttonLocked) {        // check if button has been pressed and is not currently locked out
        Serial.println("Button pressed");
        lastPressTime = millis();                        // record the time the button was pressed
        buttonLocked = true;                             // lock out the button
      } else if (buttonState == HIGH && buttonLocked) {  // check if button has been released and is currently locked out
        Serial.println("Button released");
        buttonLocked = false;  // unlock the button
      }
    }
  }
  lastButtonState = reading;  // update last button state

  if (buttonLocked && (millis() - lastPressTime) > 1000) {  // check if the button is currently locked out and the lockout timer has expired
    buttonLocked = false;                                   // unlock the button
  }

  if (buttonLocked) Serial.println("button locked");
}