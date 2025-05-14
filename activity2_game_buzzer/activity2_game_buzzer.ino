
int btn1 = 2;
int led1 = 3;

int btn2 = 4;
int led2 = 5;

int buzzer = 6;
void setup() {
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);


  Serial.begin(9600);
}

void loop() {
  if (digitalRead(btn1) == HIGH) {
    digitalWrite(led1, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(5000);
    digitalWrite(led1, LOW);
    digitalWrite(buzzer, LOW);
  } else if (digitalRead(btn2) == HIGH) {
    digitalWrite(led2, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(5000);
    digitalWrite(led2, LOW);
    digitalWrite(buzzer, LOW);
  }

}
