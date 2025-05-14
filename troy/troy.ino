// Include the AccelStepper Library
#include <AccelStepper.h>

// Define step constant
#define MotorInterfaceType 4

// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);


#define BTN_1 2
#define BTN_2 3
#define BTN_3 4

#define LED_1 5
#define LED_2 6
#define LED_3 7

void setup() {
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

	myStepper.setMaxSpeed(1000.0);
	myStepper.setAcceleration(50.0);
	myStepper.setSpeed(200);
	myStepper.moveTo(2038);

  Serial.begin(9600);

}

void loop() {

  if (digitalRead(BTN_1) == HIGH) {
    Serial.println("1");
    digitalWrite(LED_1, HIGH);
    
    if (myStepper.distanceToGo() == 0) {
      myStepper.moveTo(-myStepper.currentPosition());
    }

    myStepper.run();


    delay(5000);
    digitalWrite(LED_1, LOW);

  } else if (digitalRead(BTN_2) == HIGH) {
    Serial.println("2");

    digitalWrite(LED_2, HIGH);

    delay(10000);

    digitalWrite(LED_2, LOW);

  } else if (digitalRead(BTN_3) == HIGH) {
    Serial.println("3");


    while (true) {}
  }
}
