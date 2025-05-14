#define BTN_1 2
#define BTN_2 3
#define BTN_3 4
#define LED_1 8
#define LED_2 9
#define LED_3 10
#define FAN 5

void setup() {
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(FAN, OUTPUT);
}

void loop() {

  if (digitalRead(BTN_1) == HIGH) {
    digitalWrite(FAN, HIGH);
    digitalWrite(LED_1, HIGH);
    delay(5000);
    digitalWrite(FAN, LOW);
    digitalWrite(LED_1, LOW);
  } 
  else if (digitalRead(BTN_2) == HIGH) {
    digitalWrite(FAN, HIGH);
    digitalWrite(LED_2, HIGH);
    delay(10000);
    digitalWrite(FAN, LOW);
    digitalWrite(LED_2, LOW);
  } 
  else if (digitalRead(BTN_3) == HIGH) {
    digitalWrite(FAN, HIGH);
    digitalWrite(LED_3, HIGH);
    while (true) {}
  }

}
