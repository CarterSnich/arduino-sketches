/*
 *	Read pentiomter value and use it 
 *  for controlling LED bulb brightness.
 */ 


// constants
const int ledPin = 10;

void setup()
{
	// put your setup code here, to run once:

	pinMode(ledPin, OUTPUT);

	Serial.begin(9600);
}

void loop()
{
	// put your main code here, to run repeatedly:

	// store value of pin A0
	int sensorValue = analogRead(A0);
	int val = 0;

	// apply value for led

	if (sensorValue == 0)
	{
		val = 0;
	}
	else if (sensorValue > 0 && sensorValue <= 102)
	{
		val = 25;
	}
	else if (sensorValue >= 103 && sensorValue <= 204)
	{
		val = 50;
	}
	else if (sensorValue >= 205 && sensorValue <= 303)
	{
		val = 75;
	}
	else if (sensorValue >= 304 && sensorValue <= 405)
	{
		val = 100;
	}
	else if (sensorValue >= 406 && sensorValue <= 507)
	{
		val = 125;
	}
	else if (sensorValue >= 508 && sensorValue <= 609)
	{
		val = 150;
	}
	else if (sensorValue >= 610 && sensorValue <= 711)
	{
		val = 175;
	}
	else if (sensorValue >= 712 && sensorValue <= 813)
	{
		val = 200;
	}
	else if (sensorValue >= 814 && sensorValue <= 915)
	{
		val = 225;
	}
	else if (sensorValue >= 916)
	{
		val = 255;
	}

	analogWrite(ledPin, val);

	// print value
	Serial.print("Sensor: ");
	Serial.println(sensorValue);

	Serial.print("Output: ");
	Serial.println(val);
	
	Serial.println();
}
