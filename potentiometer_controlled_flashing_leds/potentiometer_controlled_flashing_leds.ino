
// Constants
const int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Variables
int analogVal;
int i = 0;
bool reverse = false;

void setup()
{ 
  for (int i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
}

void loop()
{
  if (i == 0) reverse = false;
  if (i == 7) reverse = true;
  
  analogVal = analogRead(A0);
  
  digitalWrite(ledPins[i], HIGH);
  delay(analogVal);
  digitalWrite(ledPins[i], LOW);

  reverse ? --i : ++i;
}
