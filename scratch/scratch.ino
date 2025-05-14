void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i, j;

  for (i = 1; i <= 5; i++) {
    for (j = 1; j <= i; j++) {
      Serial.print(j);
      Serial.print(" ");
    }
    Serial.println();
  }

  delay(2000);
}
