
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

//int16_t framebuffer[FRAME_WIDTH][FRAME_HEIGHT];


const PROGMEM int16_t player_texture_[] = {
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK,
  ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE,
  ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK, ST77XX_WHITE, ST77XX_BLACK
};

const PROGMEM int16_t player_texture[] = {
  RGB(0,0,0), RGB(0,0,20), RGB(0,0,40), RGB(0,0,60), RGB(0,0,80), RGB(0,0,100), RGB(0,0,120), RGB(0,0,140), RGB(0,0,160), RGB(0,0,180), RGB(0,0,200), RGB(0,0,220),
  RGB(0,0,0), RGB(0,0,20), RGB(0,0,40), RGB(0,0,60), RGB(0,0,80), RGB(0,0,100), RGB(0,0,120), RGB(0,0,140), RGB(0,0,160), RGB(0,0,180), RGB(0,0,200), RGB(0,0,220),
  RGB(20,0,0), RGB(20,0,20), RGB(20,0,40), RGB(20,0,60), RGB(20,0,80), RGB(20,0,100), RGB(20,0,120), RGB(20,0,140), RGB(20,0,160), RGB(20,0,180), RGB(20,0,200), RGB(20,0,220),
  RGB(40,0,0), RGB(40,0,20), RGB(40,0,40), RGB(40,0,60), RGB(40,0,80), RGB(40,0,100), RGB(40,0,120), RGB(40,0,140), RGB(40,0,160), RGB(40,0,180), RGB(40,0,200), RGB(40,0,220),
  RGB(60,0,0), RGB(60,0,20), RGB(60,0,40), RGB(60,0,60), RGB(60,0,80), RGB(60,0,100), RGB(60,0,120), RGB(60,0,140), RGB(60,0,160), RGB(60,0,180), RGB(60,0,200), RGB(60,0,220),
  RGB(80,0,0), RGB(80,0,20), RGB(80,0,40), RGB(80,0,60), RGB(80,0,80), RGB(80,0,100), RGB(80,0,120), RGB(80,0,140), RGB(80,0,160), RGB(80,0,180), RGB(80,0,200), RGB(80,0,220),
  RGB(100,0,0), RGB(100,0,20), RGB(100,0,40), RGB(100,0,60), RGB(100,0,80), RGB(100,0,100), RGB(100,0,120), RGB(100,0,140), RGB(100,0,160), RGB(100,0,180), RGB(100,0,200), RGB(100,0,220),
  RGB(120,0,0), RGB(120,0,20), RGB(120,0,40), RGB(120,0,60), RGB(120,0,80), RGB(120,0,100), RGB(120,0,120), RGB(120,0,140), RGB(120,0,160), RGB(120,0,180), RGB(120,0,200), RGB(120,0,220),
  RGB(140,0,0), RGB(140,0,20), RGB(140,0,40), RGB(140,0,60), RGB(140,0,80), RGB(140,0,100), RGB(140,0,120), RGB(140,0,140), RGB(140,0,160), RGB(140,0,180), RGB(140,0,200), RGB(140,0,220),
  RGB(160,0,0), RGB(160,0,20), RGB(160,0,40), RGB(160,0,60), RGB(160,0,80), RGB(160,0,100), RGB(160,0,120), RGB(160,0,140), RGB(160,0,160), RGB(160,0,180), RGB(160,0,200), RGB(160,0,220),
  RGB(180,0,0), RGB(180,0,20), RGB(180,0,40), RGB(180,0,60), RGB(180,0,80), RGB(180,0,100), RGB(180,0,120), RGB(180,0,140), RGB(180,0,160), RGB(180,0,180), RGB(180,0,200), RGB(180,0,220),
  RGB(200,0,0), RGB(200,0,20), RGB(200,0,40), RGB(200,0,60), RGB(200,0,80), RGB(200,0,100), RGB(200,0,120), RGB(200,0,140), RGB(200,0,160), RGB(200,0,180), RGB(200,0,200), RGB(200,0,220),
  RGB(220,0,0), RGB(220,0,20), RGB(220,0,40), RGB(220,0,60), RGB(220,0,80), RGB(220,0,100), RGB(220,0,120), RGB(220,0,140), RGB(220,0,160), RGB(220,0,180), RGB(220,0,200), RGB(220,0,220),
  RGB(240,0,0), RGB(240,0,20), RGB(240,0,40), RGB(240,0,60), RGB(240,0,80), RGB(240,0,100), RGB(240,0,120), RGB(240,0,140), RGB(240,0,160), RGB(240,0,180), RGB(240,0,200), RGB(240,0,220),
  RGB(255,0,0), RGB(255,0,20), RGB(255,0,40), RGB(255,0,60), RGB(255,0,80), RGB(255,0,100), RGB(255,0,120), RGB(255,0,140), RGB(255,0,160), RGB(255,0,180), RGB(255,0,200), RGB(255,0,220),
  RGB(255,0,0), RGB(255,0,20), RGB(255,0,40), RGB(255,0,60), RGB(255,0,80), RGB(255,0,100), RGB(255,0,120), RGB(255,0,140), RGB(255,0,160), RGB(255,0,180), RGB(255,0,200), RGB(255,0,220),
};


typedef struct _sprite sprite;
struct _sprite {
  sprite* next;
  uint8_t x;
  uint8_t y;
  uint8_t px;
  uint8_t py;
  uint8_t width;
  uint8_t height;
  int16_t* texture;
};

sprite* mysprite;

sprite* sprite_stack;



void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0, 96, 160, 32, ST77XX_WHITE);

  mysprite = (sprite*) calloc(1, sizeof(sprite));
  mysprite->width = 12;
  mysprite->height = 16;
  mysprite->texture = player_texture;

  //int16_t *p;
  //p = player_texture;

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

  
  updateRender();

  pplayerX = playerX;
  pplayerY = playerY;
}


void updateRender() {
  if (pplayerX != playerX || pplayerY != playerY) {
    //tft.fillRect(pplayerX, pplayerY, 12, 16, ST77XX_BLACK);
    //tft.fillRect(playerX, playerY, 12, 16, ST77XX_RED);
    
    /*for (int i = 0; i < mysprite->width * mysprite->height; i ++) {
      if (!pointInRect(pplayerX + i%mysprite->width, pplayerY + floor(i/mysprite->width), playerX + i%mysprite->width, playerY + floor(i/mysprite->width), mysprite->width, mysprite->height)) {
        tft.drawPixel(pplayerX + i%mysprite->width, pplayerY + floor(i/mysprite->width), ST77XX_BLACK);
      }
    }*/


    for (int i = 0; i < 12; i ++) {
      for (int j = 0; j < 16; j ++) {
        if (!pointInRect(pplayerX + i, pplayerY + j, playerX, playerY, 12, 16)) {
          tft.drawPixel(pplayerX + i, pplayerY + j, ST77XX_BLACK);
        }
      }
    }

    for (int i = 0; i < mysprite->width * mysprite->height; i ++) {
      tft.drawPixel(playerX + i%mysprite->width, playerY + floor(i/mysprite->width), pgm_read_word_near(mysprite->texture + i));
    }
  }
}



boolean pointInRect(uint8_t px, uint8_t py, uint8_t rx, uint8_t ry, uint8_t rw, uint8_t rh) {
  return px >= rx && px < rx + rw && py >= ry && py < ry + rh;
}


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
