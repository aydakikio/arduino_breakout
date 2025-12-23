#include <U8g2lib.h>

// ===== HARDWARE CONFIGURATION =====
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, -1, A5, A4);

#define BTN_RIGHT 10
#define BTN_LEFT 7

#define PEDAL_SPEED 2

struct Block{
  int x;
  int y;
  int w;
  int h;
  bool active;    
};

struct Ball{
  int x;
  int y;

};

Block player_pedal;
Ball ball;

// forward declaration
void draw_game();
void initialize_game();

void setup() {
  u8g2.begin();
  
  // intialising buttons 
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);

  initialize_game();
}

void loop() {
  //u8g2.clearBuffer();

  //u8g2.setFont(u8g2_font_7x13_mf);
  //u8g2.drawStr(6, 32, "hi");
  draw_game();
  //u8g2.sendBuffer();
}

void initialize_game(){
  // Init player's pedal
  player_pedal.x=32;
  player_pedal.y=120;
  player_pedal.active=true;
  player_pedal.w=10;
  player_pedal.h=3;

}

void handle_pedal_controls(){

  if(digitalRead(BTN_LEFT) == LOW){
    player_pedal.x -= PEDAL_SPEED;
  }

  if(digitalRead(BTN_RIGHT) == LOW){
    player_pedal.x += PEDAL_SPEED;
  }
}

void draw_game(){
  u8g2.clearBuffer();

  handle_pedal_controls();

  u8g2.drawBox(player_pedal.x, player_pedal.y , player_pedal.w, player_pedal.h);

  u8g2.sendBuffer();
}