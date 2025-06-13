#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define KEYPAD_A 28
#define KEYPAD_B 27
#define KEYPAD_UP 29
#define KEYPAD_RIGHT 26
#define KEYPAD_DOWN 15
#define KEYPAD_LEFT 14
#define SPEAKER_PIN 9


// Define constants
const int ENEMY_COUNT = 3;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int GAME_AREA_WIDTH = 100; // Game area width
const int SCORE_AREA_WIDTH = SCREEN_WIDTH - GAME_AREA_WIDTH; // Score area width
const int TANK_WIDTH = 6; // Tank width reduced by 5 times
const int TANK_HEIGHT = 6; // Tank height reduced by 5 times
const int BULLET_SPEED = 5; // Bullet speed
const int PLAYER_SHOT_INTERVAL = 1000; // Player tank firing interval in milliseconds
const int LIVES = 3; // Initial number of player tank lives

// Define I2C address and reset pin
#define OLED_RESET    -1 // Use default reset pin
#define SCREEN_ADDRESS 0x3C

// Initialize OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define direction enumeration
enum Direction { LEFT, RIGHT, UP, DOWN };

// Enemy structure
struct Enemy {
  int x, y; // Position
  int dx, dy; // Increment per frame
  Direction dir; // Current moving direction
  unsigned long lastShotTime; // Last firing time
  bool active; // Whether the enemy is active
};

// Bullet structure
struct Bullet {
  int x, y; // Position
  int dx, dy; // Increment per frame
  Direction dir; // Bullet direction
  bool active; // Whether activated
};

// Global variables
Enemy enemies[ENEMY_COUNT];
Bullet bullets[10]; // Assume a maximum of 10 bullets at the same time
int playerX = GAME_AREA_WIDTH / 2 - TANK_WIDTH / 2; // Initial position of the player tank
int playerY = SCREEN_HEIGHT - TANK_HEIGHT - 1;
Direction playerDir = UP; // Initial direction of the player tank
unsigned long lastPlayerShotTime = 0; // Last firing time of the player tank
int score = 0; // Score
int lives = LIVES; // Player tank lives

void setup() {
  // Initialize serial communication for debugging information
  Serial.begin(9600);

  pinMode(KEYPAD_A, INPUT_PULLUP);
  pinMode(KEYPAD_B, INPUT_PULLUP);
  pinMode(KEYPAD_UP, INPUT_PULLUP);
  pinMode(KEYPAD_LEFT, INPUT_PULLUP);
  pinMode(KEYPAD_DOWN, INPUT_PULLUP);
  pinMode(KEYPAD_RIGHT, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  
  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // If initialization fails, enter an infinite loop
  }
  display.clearDisplay();


  // Show splash screen
  showSplashScreen();

  // Initialize enemies
  for (int i = 0; i < ENEMY_COUNT; i++) {
    spawnEnemy(i);
  }

  // Initialize bullets
  for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
    bullets[i].active = false;
  }
}

void loop() {
  // Clear screen
  display.clearDisplay();

  // Detect button input to control player tank movement
  if (digitalRead(KEYPAD_UP) == LOW) { // Assume pin 2 is connected to the up direction key
    playerDir = UP;
    playerY = max(playerY - 1, 0);
  } else if (digitalRead(KEYPAD_DOWN) == LOW) { // Assume pin 3 is connected to the down direction key
    playerDir = DOWN;
    playerY = min(playerY + 1, SCREEN_HEIGHT - TANK_HEIGHT);
  } else if (digitalRead(KEYPAD_LEFT) == LOW) { // Assume pin 4 is connected to the left direction key
    playerDir = LEFT;
    playerX = max(playerX - 1, 0);
  } else if (digitalRead(KEYPAD_RIGHT) == LOW) { // Assume pin 5 is connected to the right direction key
    playerDir = RIGHT;
    playerX = min(playerX + 1, GAME_AREA_WIDTH - TANK_WIDTH);
  }

  // Automatically fire player tank's bullets
//  if (millis() - lastPlayerShotTime > PLAYER_SHOT_INTERVAL) {
  if (digitalRead(KEYPAD_A) == LOW) {
    fireBullet(playerX + TANK_WIDTH / 2, playerY + TANK_HEIGHT / 2, playerDir);
    lastPlayerShotTime = millis();
  }

  // Move enemies
  moveEnemies();
  
  // Automatically fire enemy bullets
  autoFireEnemies(); 

  // Update and draw bullets
  updateBullets();
  drawBullets();

  // Check collisions
  checkCollisions();

  // Draw player tank
  drawMiniTank(playerX, playerY, playerDir, 0);

  // Draw enemies
  drawEnemies();

  // Draw vertical line to separate game area and score area
  display.drawLine(GAME_AREA_WIDTH, 0, GAME_AREA_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  // Show score
  displayScore();

  // Display content
  display.display();

  // If player tank's lives are used up, restart the game
  if (lives <= 0) {
    resetGame();
  }

  // Wait for a while to simulate frame rate
  delay(50);
}

void showSplashScreen() {
  // Set font size
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  // Clear screen
  display.clearDisplay();

  // Calculate the width and height of "Tank" text
  int16_t x1, y1;
  uint16_t w1, h1;
  display.getTextBounds("Tank", 0, 0, &x1, &y1, &w1, &h1);

  // Calculate the width and height of "Battle" text
  int16_t x2, y2;
  uint16_t w2, h2;
  display.getTextBounds("Battle", 0, 0, &x2, &y2, &w2, &h2);

  // Calculate the total height of the two lines of text
  uint16_t totalHeight = h1 + h2 + 4; // Add some spacing

  // Calculate the starting y-coordinate to center the two lines of text
  int16_t startY = (SCREEN_HEIGHT - totalHeight) / 2;

  // Display "Tank" text, centered
  display.setCursor((SCREEN_WIDTH - w1) / 2, startY);
  display.println("Tank");

  // Display "Battle" text, centered
  display.setCursor((SCREEN_WIDTH - w2) / 2, startY + h1 + 4); // Add the height of the previous line and spacing
  display.println("Battle");

  // Update display
  display.display();

  // Pause for a while to allow the user to see the splash screen
  delay(2000);
}

void drawMiniTank(int x, int y, Direction dir, int rotation) {
  // Define the position and size of the tank's body and turret (reduced by 5 times)
  int turretWidth = 2; // Turret width reduced by 5 times
  int turretHeight = 2; // Turret height reduced by 5 times

  // Calculate the turret's position based on the tank's moving direction
  int turretX, turretY;
  switch (dir) {
    case LEFT:
      turretX = x - turretWidth / 2;
      turretY = y + (TANK_HEIGHT - turretHeight) / 2;
      break;
    case RIGHT:
      turretX = x + TANK_WIDTH - turretWidth / 2;
      turretY = y + (TANK_HEIGHT - turretHeight) / 2;
      break;
    case UP:
      turretX = x + (TANK_WIDTH - turretWidth) / 2;
      turretY = y - turretHeight;
      break;
    case DOWN:
      turretX = x + (TANK_WIDTH - turretWidth) / 2;
      turretY = y + TANK_HEIGHT - turretHeight / 2;
      break;
  }

  // Draw the tank's body (square)
  display.fillRect(x, y, TANK_WIDTH, TANK_HEIGHT, SSD1306_WHITE);

  // Draw the tank's turret (small rectangle above the body)
  display.fillRect(turretX, turretY, turretWidth, turretHeight, SSD1306_WHITE);
}

void moveEnemies() {
  for (int i = 0; i < ENEMY_COUNT; i++) {
    if (enemies[i].active) {
      // Move the enemy
      enemies[i].x += enemies[i].dx;
      enemies[i].y += enemies[i].dy;

      // If the enemy moves out of the screen, re-enter from the other side
      if (enemies[i].x > GAME_AREA_WIDTH || enemies[i].x < 0) {
        enemies[i].dx *= -1; // Reverse direction
        enemies[i].dir = (enemies[i].dx > 0) ? RIGHT : LEFT;
      }
      if (enemies[i].y > SCREEN_HEIGHT || enemies[i].y < 0) {
        enemies[i].dy *= -1; // Reverse direction
        enemies[i].dir = (enemies[i].dy > 0) ? DOWN: UP;
}
}
}
}

void autoFireEnemies() {
for (int i = 0; i < ENEMY_COUNT; i++) {
if (enemies[i].active) {
if (millis() - enemies[i].lastShotTime > PLAYER_SHOT_INTERVAL) {
fireBullet(enemies[i].x + (TANK_WIDTH / 2) - 1, enemies[i].y + (TANK_HEIGHT / 2) - 1, enemies[i].dir);
enemies[i].lastShotTime = millis();
}
}
}
}

void fireBullet(int x, int y, Direction dir) {
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
if (!bullets[i].active) {
bullets[i].x = x;
bullets[i].y = y;
bullets[i].dir = dir;
bullets[i].active = true;
setBulletSpeed(bullets[i], dir);
break;
}
}
}

void setBulletSpeed(Bullet& bullet, Direction dir) {
switch (dir) {
case LEFT:
bullet.dx = -2 * BULLET_SPEED;
bullet.dy = 0;
break;
case RIGHT:
bullet.dx = 2 * BULLET_SPEED;
bullet.dy = 0;
break;
case UP:
bullet.dx = 0;
bullet.dy = -2 * BULLET_SPEED;
break;
case DOWN:
bullet.dx = 0;
bullet.dy = 2 * BULLET_SPEED;
break;
}
}

void drawEnemies() {
for (int i = 0; i < ENEMY_COUNT; i++) {
if (enemies[i].active) {
drawMiniTank(enemies[i].x, enemies[i].y, enemies[i].dir, 0);
}
}
}

void updateBullets() {
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
if (bullets[i].active) {
bullets[i].x += bullets[i].dx;
bullets[i].y += bullets[i].dy;

  if (bullets[i].x < 0 || bullets[i].x > GAME_AREA_WIDTH || bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT) {
    bullets[i].active = false;
  }
}
}
}

void drawBullets() {
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
if (bullets[i].active) {
display.fillRect(bullets[i].x, bullets[i].y, 2, 2, SSD1306_WHITE);
}
}
}

void checkCollisions() {
// Check for collisions between player bullets and enemies
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
if (bullets[i].active && bullets[i].dir == playerDir) { // Ensure it's the player's bullet
for (int j = 0; j < ENEMY_COUNT; j++) {
if (enemies[j].active && bullets[i].x >= enemies[j].x && bullets[i].x < enemies[j].x + TANK_WIDTH &&
bullets[i].y >= enemies[j].y && bullets[i].y < enemies[j].y + TANK_HEIGHT) {
// Player bullet hits enemy
bullets[i].active = false;
enemies[j].active = false;
score++; // Increase score
spawnEnemy(j); // Spawn a new enemy tank
break; // Exit inner loop
}
}
}
}

// Check for collisions between enemy bullets and player tank
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
if (bullets[i].active && bullets[i].dir != playerDir) { // Ensure it's the enemy's bullet
if (bullets[i].x >= playerX && bullets[i].x < playerX + TANK_WIDTH &&
bullets[i].y >= playerY && bullets[i].y < playerY + TANK_HEIGHT) {
// Enemy bullet hits player tank
bullets[i].active = false;
lives--; // Player tank loses a life
if (lives <= 0) {
resetGame(); // If lives are used up, restart the game
}
}
}
}
}

void spawnEnemy(int index) {
enemies[index].active = true;
if (index < ENEMY_COUNT / 2) {
enemies[index].x = random(0, GAME_AREA_WIDTH / 2);
enemies[index].y = random(0, SCREEN_HEIGHT);
enemies[index].dx = 1;
enemies[index].dy = 0;
enemies[index].dir = RIGHT;
} else {
enemies[index].x = random(0, GAME_AREA_WIDTH);
enemies[index].y = random(0, SCREEN_HEIGHT / 2);
enemies[index].dx = 0;
enemies[index].dy = 1;
enemies[index].dir = DOWN;
}
enemies[index].lastShotTime = millis() - PLAYER_SHOT_INTERVAL; // Ensure immediate firing
fireBullet(enemies[index].x + (TANK_WIDTH / 2) - 1, enemies[index].y + (TANK_HEIGHT / 2) - 1, enemies[index].dir);
}

void displayScore() {
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);

display.setCursor(GAME_AREA_WIDTH + 5, 0);
display.print("PTS");
display.setCursor(GAME_AREA_WIDTH + 10, 16);
display.println(score);

display.setCursor(GAME_AREA_WIDTH + 7, 32);
display.print("LV");
display.setCursor(GAME_AREA_WIDTH + 10, 48);
display.println(lives);
}

void resetGame() {
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);

int16_t x, y;
uint16_t w, h;
display.getTextBounds("Game Over", 0, 0, &x, &y, &w, &h);
display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
display.println("Game Over");
display.display();
delay(3000);

for (int i = 0; i < ENEMY_COUNT; i++) {
spawnEnemy(i);
}
for (int i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++) {
bullets[i].active = false;
}
playerX = GAME_AREA_WIDTH / 2 - TANK_WIDTH / 2;
playerY = SCREEN_HEIGHT - TANK_HEIGHT - 1;
playerDir = UP;
score = 0;
lives = LIVES;
}
