#include "modules/controls.cpp"
#include "modules/LEDS.cpp"
controlPanel controls;
leds traffic;
int SequenceInterval = 1000;  // 1 second delay;
unsigned long CurrentMSInterval;
bool executeSeq = false;
void setup() {
  // put your setup code here, to run once:
  controls.init();
  traffic.init();
}
void loop() {
  // put your main code here, to run repeatedly:
  //Listen to Key Press
  controls.onKeyPress();
  if (controls.isPressed()) {
    if (executeSeq) {
      executeSeq = false;
    } else {
      //bool to run sequence if Toggled true
      executeSeq = true;
    }
  } else if (controls.isLongPress) {
    //execute every SequenceInterval for Long Press
    if (millis() - CurrentMSInterval >= SequenceInterval) {
      CurrentMSInterval = millis();
      setLights();
    }
    executeSeq = false;
  } else if (executeSeq) {
    //execute every SequenceInterval on Toggle
    if (millis() - CurrentMSInterval >= SequenceInterval) {
      //Serial.println("Long Press Occuring!");
      CurrentMSInterval = millis();
      setLights();
    }
  } else {
    //Reset Leds and Sequences if nothing fits above.
    traffic.resetLeds();
    traffic.SequenceIndex = 0;
    executeSeq = false;
  }
}
//Function Helper to Toggle LEDS
void setLights() {
  //Turn Off LEDs if rapid press have been Detected
  if (controls.rapidPress) {
    executeSeq = false;
    traffic.SequenceIndex = 0;
    return;
  }
  traffic.setLeds();
  return;
}