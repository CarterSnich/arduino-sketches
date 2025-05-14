

class mxpatterns {

public:
  bool scanBuffer[8][8 * 5];
public:
  String textBuff = "";


public:
  bool Patterns[8][8] = {
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 1, 0, 1, 0, 1, 1 },
    { 1, 1, 0, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0, 1, 1 },
    { 1, 1, 0, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0, 1, 1 },
    { 1, 1, 0, 1, 0, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 }
  };

  // letters

  //Define Letters Here! put 1 or 0 to to make a pattern

public:
  bool A[8][8] = {
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 },
    { 0, 1, 0, 0, 0, 0, 1, 0 }
  };
public:
  bool B[8][8] = {
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 }
  };


public:
  bool C[8][8] = {
    { 0, 0, 0, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 0, 0 }
  };






public:
  bool blank[8][8] = {
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 }
  };


public:
  bool empty[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
  };






  //Letters end


public:
  void writeBuffer(bool let[8][8], int place) {

    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        scanBuffer[y][x + (8 * place)] = let[y][x];
      }
    }


    return;
  }





  int offset = 0;
  int size;


public:
  void scrollChars() {
    return;

    size = textBuff.length();
    if (size > 3) {
      size = 3;
    }

    //pagenated(buffs[0], 0);

    for (int y = 0; y < 8; y++) {


      for (int x = 0; x < 8; x++) {  //Scan trough the colms

        Patterns[y][x] = scanBuffer[y][x + offset];
      }
    }

    offset++;


    if (offset > (size * 8) + 1 || offset >= 40) {
      offset = 0;
    }
  }


  int buffWrite = 0;

  //Write a specfic letter on a specific index of the buffer
public:
  void writeChar(char ch, int index) {



    switch (ch) {
      case 'A':
        writeBuffer(A, index);

        break;

      case 'B':
        writeBuffer(B, index);

        break;


      case 'C':
        writeBuffer(C, index);

        break;



      default:
        writeBuffer(blank, index);

        break;
    }
  }

  int nextl = 3;

public:
  void nextLine() {


    if (textBuff.length() > 3) {

      //writeChar(text.charAt(c), c);
      for (int c = 0; c < size; c++) {

        if (String(textBuff.charAt(c + nextl)).length()) {
          writeChar(textBuff.charAt(c + nextl), c);
          Serial.println(String(textBuff.charAt(c + nextl)).length());
        } else {
          writeBuffer(empty, c);
        }
      }

      nextl = nextl + 3;

      if (nextl > textBuff.length()) {

        nextl = 0;
      }
    }
  }

public:
  void text(String txt) {

    textBuff = txt;

    int limit = 0;

    limit = txt.length();

    if (limit > 3) {
      limit = 3;
    }

    for (int c = 0; c < limit; c++) {
      writeChar(textBuff.charAt(c), c);
    }
  }
};