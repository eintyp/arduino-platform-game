
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define RGB(r, g, b) (((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3))

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

#define FRAME_RATE 30
#define FRAME_WIDTH 160
#define FRAME_HEIGHT 128
#define PLAYER_SPRITE_WIDTH 12
#define PLAYER_SPRITE_HEIGHT 16

Adafruit_ST7735 tft = Adafruit_ST7735(10, 8, -1);

uint8_t BUTTON_LAST = BUTTON_NONE;
unsigned long ticks = 0;

int playerX = 16, playerY = 80;
int pplayerX = playerX, pplayerY = playerY;
float playerAccX, playerAccY;

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  /*
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(4, 4, 120, 152, 8, ST77XX_WHITE);
  tft.drawRoundRect(6, 6, 116, 148, 6, ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(12, 12);
  tft.println("You may now");
  tft.setCursor(12, 20);
  tft.println("switch off your");
  tft.setCursor(12, 28);
  tft.println("Macintosh safely.");
  redrawButtons();
  */
  
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0, 96, 160, 32, ST77XX_WHITE);
}

void loop() {
  if (millis() / 1000.0 * FRAME_RATE > ticks) {
    ticks++;
    tick();
  }
}


void tick() {
  uint8_t c = readController();

  if (playerY < 80) playerAccY += 1;
  
  //if (c != BUTTON_LAST) {
    switch (c) {
      case BUTTON_RIGHT:
      playerAccX += 2;
      break;
      case BUTTON_LEFT:
      playerAccX -= 2;
      break;
      case BUTTON_UP:
      if (c != BUTTON_LAST) playerAccY = -8;
      break;
      case BUTTON_SELECT:
      break;
    }
  //}

  /*if (c != BUTTON_RIGHT && c != BUTTON_LEFT && playerAccX != 0) {
    int brake = c == BUTTON_NONE ? 3 : 1;*/
  if (c == BUTTON_NONE && playerAccX != 0) {
    int brake = 3;
    if (playerAccX > 0) playerAccX -= brake;
    else playerAccX += brake;
    
    if (abs(playerAccX) < brake) playerAccX = 0;
  }
  
  BUTTON_LAST = c;

  playerAccX = constrain(playerAccX, -8, 8);
  playerX += playerAccX;
  if (playerX <= 0 || playerX >= FRAME_WIDTH - PLAYER_SPRITE_WIDTH) {
    playerX = constrain(playerX, 0, FRAME_WIDTH - PLAYER_SPRITE_WIDTH);
    playerAccX = 0;
  }
  
  playerY += playerAccY;
  if (playerY >= 80) {
    playerY = 80;
    playerAccY = 0;
  }
  

  if (pplayerX != playerX || pplayerY != playerY) {
    tft.fillRect(pplayerX, pplayerY, 12, 16, ST77XX_BLACK);
    tft.fillRect(playerX, playerY, 12, 16, ST77XX_RED);
  }

  pplayerX = playerX;
  pplayerY = playerY;
}

/*
void drawButton(String text, int y, boolean selected) {
  if (selected) {
    tft.fillRoundRect(12, y, 104, 10, 6, ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(16, y + 1);
    tft.println(text);
  } else {
    tft.fillRoundRect(12, y, 104, 10, 6, ST77XX_WHITE);
    tft.drawRoundRect(12, y, 104, 10, 6, ST77XX_BLACK);
    tft.setTextColor(ST77XX_BLACK);
    tft.setCursor(16, y + 1);
    tft.println(text);
  }
}

void redrawButtons() {
  drawButton("restart", 40, (selected == 0));
  drawButton("restart", 52, (selected == 1));
  drawButton("restart", 64, (selected == 2));
  drawButton("restart", 76, (selected == 3));
  drawButton("restart", 88, (selected == 4));
  drawButton("restart", 100, (selected == 5));
  drawButton("restart", 112, (selected == 6));
  drawButton("restart", 124, (selected == 7));
}*/


uint8_t readController(void) {
  float a = analogRead(3);
  
  a *= 5.0;
  a /= 1024.0;
  
  if (a < 0.2) return BUTTON_RIGHT;
  if (a < 1.0) return BUTTON_UP;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_LEFT;
  if (a < 3.2) return BUTTON_DOWN;
  else return BUTTON_NONE;
}
