/* 8 x 8 Matrix LED Canvas Module Class by
Atomtech 2023
*/


class Mcanvas {
  int Cols[8] = {};
  int ClockPin;
  int ResetPin;

  bool buffer[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
  };

  //Initialize the Cols pins and the clock and reset for CD4017
public:
  void init(int colPins[8], int clock, int reset) {

    for (int x = 0; x < 8; x++) {
      Cols[x] = colPins[x];
      pinMode(Cols[x], OUTPUT);
      digitalWrite(Cols[x], HIGH);
    }

    pinMode(clock, OUTPUT);
    pinMode(reset, OUTPUT);
    digitalWrite(clock, LOW);
    digitalWrite(reset, LOW);

    ClockPin = clock;
    ResetPin = reset;
  };

  bool writeDone = false;
  int rowI = 0;
  unsigned long prevOf;


  //Call every loop to paint into the matrix
public:
  void paint() {
    int bval;


    if (millis() - prevOf <= 1) {
      return;
    } else {
      prevOf = millis();
    }

    digitalWrite(Cols[0], HIGH);
    digitalWrite(Cols[1], HIGH);
    digitalWrite(Cols[2], HIGH);
    digitalWrite(Cols[3], HIGH);
    digitalWrite(Cols[4], HIGH);
    digitalWrite(Cols[5], HIGH);
    digitalWrite(Cols[6], HIGH);
    digitalWrite(Cols[7], HIGH);


    //     triggers next clock
    digitalWrite(ClockPin, 1);
    digitalWrite(ClockPin, 0);


    if (rowI > 7) {
      //       reset clock
      rowI = 0;
      digitalWrite(ResetPin, 1);
      digitalWrite(ResetPin, 0);
    }


    for (int col = 0; col < 8; col++) {
      bval = buffer[rowI][col];
      if (bval) {
        digitalWrite(Cols[col], LOW);
      } else {
        digitalWrite(Cols[col], HIGH);
      }
    }

    rowI++;
  }

public:
  void draw(bool dataArray[8][8]) {


    for (int y = 0; y <= 7; y++) {
      for (int x = 0; x <= 7; x++) {
        buffer[y][x] = dataArray[y][x];
      }
    }
  }
};
