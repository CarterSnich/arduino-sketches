#include <SoftwareSerial.h>

#include "modules/mtcanvas.cpp"
#include "modules/mxpatterns.cpp"

const int colPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

Mcanvas canvas;
mxpatterns pattern;

bool picture[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};


SoftwareSerial espSerial(0, 1);  // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);
  canvas.init(colPins, 10, 11);
};


void loop() {
  canvas.paint();

  if (espSerial.available()) {
    String str = espSerial.readString();
    String delimiter = "|";
    int binaryMatrix[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    Serial.println(str);

    // Split the string into substrings
    String substrings[8];
    int index = 0;
    while (str.length() > 0) {
      int pos = str.indexOf(delimiter);
      if (pos == -1) {
        substrings[index++] = str;
        str = "";
      } else {
        substrings[index++] = str.substring(0, pos);
        str = str.substring(pos + delimiter.length());
      }
    }

    // store substrings as matrix
    for (int i = 0; i < 8; i++) {
      binaryMatrix[i] = substrings[i].toInt();
    }

    // convert the binary matrix to binary whatever
    for (int i = 0; i < 8; ++i) {
      int currenBit = binaryMatrix[i];
      int currentByte[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      for (int j = 0; j < 8; ++j) {
        int remainder = currenBit % 2;
        currenBit = currenBit / 2;
        currentByte[j] = remainder;
      }

      for (int k = 0; k < 8; ++k) {
        picture[i][k] = currentByte[k];
      }
    }

    Serial.println("picture");
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        Serial.print(picture[i][j]);
      }
      Serial.println("");
    }
  }

  canvas.draw(picture);
};
