#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include "bitmaps.h"
#include "pitches.h"
#include "constants.h"

Adafruit_SSD1306 lcd(screenWidth, screenHeight, &Wire, -1);


int difficulty = 1;

int gameState = WELCOME_STATE;

bool isWantDisplayFlickeing = false;
unsigned long lastDisplayFlickering = 0;

bool isWantCollectFruitSound = false;
bool isWantButtonPressedSound = false;
unsigned long lastBuzzerPlayed = 0;

int hiscore = 0;
int score = 0;

struct Position {
  int x, y;

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  Position& operator+=(const Position& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
};

void draw_square(Position pos, int color = WHITE) {
  lcd.fillRect(pos.x * 2, pos.y * 2, 2, 2, color);
}

bool test_position(Position pos) {
  return lcd.getPixel(pos.x * 2, pos.y * 2);
}

const Position kDirPos[4] = {
  { 0, -1 },  // Up
  { 1, 0 },   // Right
  { 0, 1 },   // Down
  { -1, 0 }   // Left
};

struct Player {
  Player() {
    reset();
  }

  Position pos;
  unsigned char tail[maxLength];
  unsigned char direction;
  int size, moved;

  void reset() {
    pos = { 32, 16 };
    direction = 1;
    size = startLength;
    memset(tail, 0, sizeof(tail));
    moved = 0;
  }

  bool isTurningDirIsVaild(int dir) {
    if (dir == TURN_LEFT && direction == TURN_RIGHT) {
      return false;
    } else if (dir == TURN_RIGHT && direction == TURN_LEFT) {
      return false;
    } else if (dir == TURN_DOWN && direction == TURN_UP) {
      return false;
    } else if (dir == TURN_UP && direction == TURN_DOWN) {
      return false;
    }

    return true;
  }

  void turn(int dir) {
    if (!isTurningDirIsVaild(dir)) return;
    direction = dir;
  }

  void update() {
    for (int i = maxLength - 1; i > 0; --i) {
      tail[i] = tail[i] << 2 | ((tail[i - 1] >> 6) & 3);
    }

    tail[0] = tail[0] << 2 | ((direction + 2) % 4);
    pos += kDirPos[direction];

    if (moved < size) {
      moved++;
    }
  }

  void render() const {
    draw_square(pos);

    if (moved < size) {
      return;
    }

    Position tailpos = pos;

    for (int i = 0; i < size; ++i) {
      tailpos += kDirPos[(tail[(i >> 2)] >> ((i & 3) * 2)) & 3];
    }

    draw_square(tailpos, BLACK);
  }
} player;

struct Item {
  Position pos;
  void spawn() {
    pos.x = random(1, 63);
    pos.y = random(1, 31);
  }

  void render() const {
    draw_square(pos);
  }
} item;

void waitForInput() {
  do {
    handelSound();
  } while (digitalRead(KEYPAD_A) == LOW);

  isWantButtonPressedSound = true;
  lastBuzzerPlayed = millis();
  handelSound();
}

void pushToStart() {
  lcd.setCursor(0, 57);
  lcd.print(F("Push to start  "));
  switch (difficulty) {
    case 0:
      lcd.print(F("HARD"));
      break;
    case 1:
      lcd.print(F("NORMAL"));
      break;
    case 2:
      lcd.print(F("EASY"));
      break;
  }
}

void playIntro() {
  lcd.clearDisplay();
  lcd.drawBitmap(18, 0, kSplashScreen, 92, 56, WHITE);
  pushToStart();
  lcd.display();
}

void playGameover() {
  lcd.clearDisplay();
  lcd.drawBitmap(4, 0, kGameOver, 124, 38, WHITE);
  lcd.setCursor(26, 34);
  lcd.print(F("Score: "));
  lcd.print(score);
  lcd.setCursor(26, 44);
  lcd.print(F("Hi-Score: "));
  lcd.print(hiscore);
  pushToStart();
  lcd.display();
}

void resetGame() {
  lcd.clearDisplay();

  for (int x = 0; x < gameWidth; ++x) {
    draw_square({ x, 0 });
    draw_square({ x, 31 });
  }
  for (int y = 0; y < gameHeight; ++y) {
    draw_square({ 0, y });
    draw_square({ 63, y });
  }
  player.reset();
  item.spawn();
}

void updateGame() {
  player.update();
}

bool isGameOver() {
  if (player.pos == item.pos) return false;
  return test_position(player.pos);
}

void input() {
  if (digitalRead(KEYPAD_UP) == LOW) {
    player.turn(TURN_UP);
  } else if (digitalRead(KEYPAD_RIGHT) == LOW) {
    player.turn(TURN_RIGHT);
  } else if (digitalRead(KEYPAD_DOWN) == LOW) {
    player.turn(TURN_DOWN);
  } else if (digitalRead(KEYPAD_LEFT) == LOW) {
    player.turn(TURN_LEFT);
  }
}

void render() {
  player.render();
  item.render();
  lcd.display();
}


void handelGameState() {
  switch (gameState) {
    case WELCOME_STATE:
      playIntro();

      if (digitalRead(KEYPAD_A) == LOW) {
        isWantDisplayFlickeing = true;
        lastDisplayFlickering = millis();
        resetGame();
        gameState = GAMEPLAY_STATE;
      } else if (digitalRead(KEYPAD_UP) == LOW && difficulty < 2) {
        ++difficulty;
        delay(100);
      } else if (digitalRead(KEYPAD_DOWN) == LOW && difficulty > 0) {
        --difficulty;
        delay(100);
      }
      break;

    case GAMEPLAY_STATE:
      input();
      player.update();

      if (player.pos == item.pos) {
        isWantCollectFruitSound = true;
        lastBuzzerPlayed = millis();
        player.size++;
        item.spawn();
      } else if (isGameOver()) {
        playBtnClickedSound();
        score = (player.size - startLength) * (3 - difficulty);
        if (score > hiscore) {
          hiscore = score;
        }
        gameState = GAMEOVER_STATE;
        isWantDisplayFlickeing = true;
        lastDisplayFlickering = millis();
      }

      render();
      break;

    case GAMEOVER_STATE:
      playGameover();

      if (digitalRead(KEYPAD_A) == LOW) {
        isWantDisplayFlickeing = true;
        lastDisplayFlickering = millis();
        resetGame();
        gameState = GAMEPLAY_STATE;
      } else if (digitalRead(KEYPAD_UP) == LOW && difficulty < 2) {
        ++difficulty;
        delay(100);
      } else if (digitalRead(KEYPAD_DOWN) == LOW && difficulty > 0) {
        --difficulty;
        delay(100);
      }

      break;
  }
}

void playBtnClickedSound() {
  isWantButtonPressedSound = true;
  lastBuzzerPlayed = millis();
}

void handelDisplayFlickring() {
  if (isWantDisplayFlickeing) {
    if (millis() <= lastDisplayFlickering + 50) {
      lcd.invertDisplay(true);
    } else {
      lcd.invertDisplay(false);
    }
  }
}

void handelSound() {
  if (isWantCollectFruitSound) {
    if (millis() <= lastBuzzerPlayed + 100) {
      tone(SPEAKER_PIN, NOTE_B5, 100);
    } else if (millis() <= lastBuzzerPlayed + 455) {
      tone(SPEAKER_PIN, NOTE_E6, 850);
    } else {
      noTone(SPEAKER_PIN);
      isWantCollectFruitSound = false;
    }
  } else if (isWantButtonPressedSound) {
    if (millis() <= lastBuzzerPlayed + 200) {
      tone(SPEAKER_PIN, NOTE_AS2, 120);
    } else {
      noTone(SPEAKER_PIN);
      isWantButtonPressedSound = false;
    }
  } else {
    noTone(SPEAKER_PIN);
  }
}

void setup() {
  pinMode(KEYPAD_A, INPUT_PULLUP);
  pinMode(KEYPAD_UP, INPUT_PULLUP);
  pinMode(KEYPAD_LEFT, INPUT_PULLUP);
  pinMode(KEYPAD_DOWN, INPUT_PULLUP);
  pinMode(KEYPAD_RIGHT, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);

  noTone(SPEAKER_PIN);

  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  lcd.setTextColor(WHITE);
}

void loop() {
  handelGameState();
  handelSound();
  handelDisplayFlickring();
  delay(difficulty * 50);
}