// #define PIN_A 6
// #define PIN_B 7

// #define DELAY 0

// int oldA = HIGH;
// int oldB = HIGH;
// int num = 0;

// void setup() {
//   pinMode(PIN_A, INPUT_PULLUP);
//   pinMode(PIN_B, INPUT_PULLUP);
//   Serial.begin(9600);
// }

// void loop() {
//   int currentA;
//   int currentB;

//   currentA = digitalRead(PIN_A);
//   if (oldA != currentA) {
//     num = num + 1;
//     Serial.println(num);
//     oldA = currentA;
//     return;
//   }

//   currentB = digitalRead(PIN_B);
//   if (oldB != currentB) {
//     num = num - 1;
//     Serial.println(num);
//     oldB = currentB;
//     return;
//   }
// }




/*
Quadrature Example
*/
int quadAPin = 1;
int quadBPin = 2;

int lastQuadValue;

int CLOCKWISE = 1;
int CCLOCKWISE = -1;
int MISSEDPULSE = 999;

void setup() {
  lastQuadValue = readQuadValue();
  Serial.begin(9600);
}

void loop() {
  int newQuadValue = readQuadValue();
  int quadDir = getQuadDir(lastQuadValue, newQuadValue);

  if (quadDir == CLOCKWISE) Serial.println("Clockwise");
  if (quadDir == CCLOCKWISE) Serial.println("Counter Clockwise");
  // if (quadDir == MISSEDPULSE) Serial.println("Missed Pulse Detected");

  lastQuadValue = newQuadValue;
}

int readQuadValue() {
  int val = digitalRead(quadAPin);
  val = val * 2 + digitalRead(quadBPin);
  return val;
}

int getQuadDir(int prevVal, int newVal) {

  //because the step order is 0, 1, 3, 2 lets do some switching around

  if (newVal == 3) {
    newVal = 2;
  } else {
    if (newVal == 2) newVal = 3;
  }

  if (prevVal == 3) {
    prevVal = 2;
  } else {
    if (prevVal == 2) prevVal = 3;
  }

  int quadDir = prevVal - newVal;

  //see if we missed a pulse (i.e. quadDir is 2 or -2)
  if (abs(quadDir) == 2) quadDir = MISSEDPULSE;

  return quadDir;
}