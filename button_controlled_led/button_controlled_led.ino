const int buttonPin = 2;
const int ledPin = 8;

void setup()
{
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    digitalWrite(ledPin, digitalRead(buttonPin));
}