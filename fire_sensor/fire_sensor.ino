const int sensor = 2;
const int ledPin = 8;

void setup()
{
    pinMode(sensor, INPUT);
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    digitalWrite(ledPin, digitalRead(sensor));
}