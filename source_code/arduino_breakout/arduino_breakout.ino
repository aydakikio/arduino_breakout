#include <U8g2lib.h>

// ===== HARDWARE CONFIGURATION =====
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, -1, A5, A4);

#define BTN_RIGHT 10
#define BTN_LEFT 7

#define PEDAL_SPEED 2

// Game States
bool gameover = false;

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
  int vx;
  int vy;
  int r;
};

Block player_pedal;
Ball ball;

// forward declaration
void draw_game();
void draw_gameover();
void initialize_game();

void setup() {
  u8g2.begin();
  
  // intialising buttons 
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);

  initialize_game();
}

void loop() {
  if(!gameover){
    draw_game();
  }else {
    draw_gameover();

    if(digitalRead(BTN_LEFT)==LOW || digitalRead(BTN_RIGHT)==LOW){


      initialize_game();
    }
  }
}

void initialize_game(){

  gameover = false;

  // Init player's pedal
  player_pedal.x=32;
  player_pedal.y=120;
  player_pedal.active=true;
  player_pedal.w=10;
  player_pedal.h=3;

  // Init ball
  ball.x = 32;
  ball.y = 80;
  ball.vx= 2;
  ball.vy= 2;
  ball.r = 3;
}

void ball_pysics(){
  // Move ball
  ball.x += ball.vx;
  ball.y += ball.vy;
  
  // Collisions with left and right walls
  if(ball.x - ball.r <= 0 || ball.x + ball.r >= 64) {
    ball.vx = -ball.vx;
    ball.x = constrain(ball.x, ball.r, 64 - ball.r); // Keep ball in bounds
  }
  
  // Collision with top wall
  if(ball.y - ball.r <= 0) {
    ball.vy = -ball.vy;
    ball.y = ball.r;
  }
  
  // Collision with bottom (pedal area)
  
  if(ball.y + ball.r >= 128) {
    gameover=true;

    return;
  }
  
  // Collision with player pedal
  if(ball.y + ball.r >= player_pedal.y && 
     ball.y + ball.r <= player_pedal.y + player_pedal.h &&
     ball.x >= player_pedal.x && 
     ball.x <= player_pedal.x + player_pedal.w) {
    ball.vy = -ball.vy;
    ball.y = player_pedal.y - ball.r;
  }
}

void handle_pedal_controls(){

  if(digitalRead(BTN_LEFT) == LOW){
    if (player_pedal.x <= 0) return;
    player_pedal.x -= PEDAL_SPEED;
  }

  if(digitalRead(BTN_RIGHT) == LOW){
    if (player_pedal.x >= 54) return;
    player_pedal.x += PEDAL_SPEED;
  }
}

void draw_game(){
  u8g2.clearBuffer();

  handle_pedal_controls();
  
  ball_pysics();

  //Draw ball
  u8g2.drawFilledEllipse(ball.x, ball.y, ball.r, ball.r);

  //Draw player pedal 
  u8g2.drawBox(player_pedal.x, player_pedal.y , player_pedal.w, player_pedal.h);

  u8g2.sendBuffer();
}

void draw_gameover(){
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_7x13_mf);
  
  u8g2.drawStr(1, 27, "GameOver");

  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(6, 62, "Press any");
  u8g2.drawStr(4, 71, "button to");
  u8g2.drawStr(10, 79, "restart!");
  
  u8g2.sendBuffer();
}