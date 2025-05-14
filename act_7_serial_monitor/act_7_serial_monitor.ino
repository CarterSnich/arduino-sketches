// Constants
const int redLed = 8;
const int yellowLed = 9;
const int greenLed = 10;

// variables
String color;

void setup()
{
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  Serial.print("\nPlease input any color of LED: ");
  while (Serial.available() == 0) {}
  color = Serial.readString();
  color.trim();
  Serial.println(color);

  if (color == "red") {
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);
  }

  if (color == "yellow") {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
  }

  if (color == "green") {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
  }

  if (color == "all") {
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, HIGH);
  }

  if (color == "none") {
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);
  }

}
