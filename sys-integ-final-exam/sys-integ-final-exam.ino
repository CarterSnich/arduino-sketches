void setup() {
  // begin Serial connection
  Serial.begin(115200);
}

void loop() {
  // skip loop if there is no Serial input available
  if (Serial.available() == 0) { return; }

  // read input from Serial
  String readStr = Serial.readString();

  // convert Serial input to int
  int value = readStr.toInt();

  // skip loop if value is not within range of 0-255
  if (value > 255 || value < 0) {
    Serial.println("Invalid input.");
    return;
  }

  // apply values to RGB module
  analogWrite(9, value); // RED
  analogWrite(10, value); // GREEN
  analogWrite(11, value); // BLUE
}