const int ledRed = 2;
const int ledYellow = 4;
const int ledGreen = 6;
const int timeDelay = 500;

void setup()
{
	// put your setup code here, to run once:
	pinMode(ledRed, OUTPUT);
	pinMode(ledYellow, OUTPUT);
	pinMode(ledGreen, OUTPUT);
}

void loop()
{
	// put your main code here, to run repeatedly:

	digitalWrite(ledRed, HIGH);
	digitalWrite(ledGreen, LOW);

	delay(timeDelay);

	digitalWrite(ledRed, LOW);
	digitalWrite(ledYellow, HIGH);

	delay(timeDelay);

	digitalWrite(ledYellow, LOW);
	digitalWrite(ledGreen, HIGH);

	delay(timeDelay);
	
}