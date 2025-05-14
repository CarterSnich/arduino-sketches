// constants
const int ledCouunt = 8;
const int leds[ledCouunt] = {2, 3, 4, 5, 6, 7, 8, 9};
const int photoresistor = A0;

// variables
int resistance;
int brightness;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < ledCouunt; i++) {
    pinMode(leds[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  resistance = analogRead(photoresistor);
  brightness = map(resistance, 300, 1023, 0, ledCouunt);

  Serial.print("Resistance: ");
  Serial.println(resistance);
  
  Serial.print("Brightnss: ");
  Serial.println(brightness);
  
  for (int led = 0; led < ledCouunt; led++) {
    if (led > brightness) {
      digitalWrite(leds[led], LOW);
    } else {
      digitalWrite(leds[led], HIGH);
    }
  }
}
