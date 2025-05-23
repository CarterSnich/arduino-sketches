
// constants
const int piezo = 8;
const int photoResistor = A0;
const float delayMultiplier = 2;

// variables
int resistance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  resistance = analogRead(photoResistor);
  Serial.print("Resistance: ");
  Serial.println(resistance);

  tone(piezo, 516.33);
  delay(200);
  noTone(piezo);
  delay(resistance * delayMultiplier);
  
}
