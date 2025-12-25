#include <U8g2lib.h>

// ===== HARDWARE CONFIGURATION =====
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, -1, A5, A4);

#define BTN_RIGHT 10
#define BTN_LEFT 7
#define PEDAL_SPEED 2

// Game configuration
#define MAX_BLOCKS 30
#define BLOCK_ROWS 5
#define BLOCK_COLS 6
#define BLOCK_WIDTH 9
#define BLOCK_HEIGHT 4
#define BLOCK_SPACING 1
#define BLOCK_START_Y 5

// Game States
bool gameover = false;
int blocks_remaining = 0;

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
Block blocks[MAX_BLOCKS];

// forward declaration
void draw_game();
void draw_gameover();
void initialize_game();
void create_blocks();
void check_block_collisions();

void setup() {
  u8g2.begin();
  // initialising buttons 
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  randomSeed(analogRead(A0)); // Seed random for angle variations
  initialize_game();
}

void loop() {
  if(!gameover){
    draw_game();
  } else {
    draw_gameover();
    if(digitalRead(BTN_LEFT)==LOW || digitalRead(BTN_RIGHT)==LOW){
      delay(200); // Debounce
      initialize_game();
    }
  }
}

void create_blocks(){
  int index = 0;
  blocks_remaining = 0;
  
  for(int row = 0; row < BLOCK_ROWS; row++){
    for(int col = 0; col < BLOCK_COLS; col++){
      if(index < MAX_BLOCKS){
        blocks[index].x = col * (BLOCK_WIDTH + BLOCK_SPACING) + 2;
        blocks[index].y = BLOCK_START_Y + row * (BLOCK_HEIGHT + BLOCK_SPACING);
        blocks[index].w = BLOCK_WIDTH;
        blocks[index].h = BLOCK_HEIGHT;
        blocks[index].active = true;
        blocks_remaining++;
        index++;
      }
    }
  }
}

void initialize_game(){
  gameover = false;
    
  // Init player's pedal
  player_pedal.x = 32;
  player_pedal.y = 120;
  player_pedal.active = true;
  player_pedal.w = 10;
  player_pedal.h = 3;
  
  // Init ball with slight random angle
  ball.x = 32;
  ball.y = 80;
  ball.vx = random(1, 3); 
  ball.vy = 2;
  ball.r = 2;
  
  // Randomly choose left or right direction
  if(random(0, 2) == 0) {
    ball.vx = -ball.vx;
  }
  
  // Create blocks
  create_blocks();
}

void check_block_collisions(){
  for(int i = 0; i < MAX_BLOCKS; i++){
    if(!blocks[i].active) continue;
    
    // Check if ball intersects with block
    if(ball.x + ball.r >= blocks[i].x && 
       ball.x - ball.r <= blocks[i].x + blocks[i].w &&
       ball.y + ball.r >= blocks[i].y && 
       ball.y - ball.r <= blocks[i].y + blocks[i].h){
      
      // Deactivate block
      blocks[i].active = false;
      blocks_remaining--;
      
      // Determine bounce direction based on collision side
      int blockCenterX = blocks[i].x + blocks[i].w / 2;
      int blockCenterY = blocks[i].y + blocks[i].h / 2;
      
      int dx = ball.x - blockCenterX;
      int dy = ball.y - blockCenterY;
      
      // Bounce vertically if hit from top/bottom
      if(abs(dy) > abs(dx)){
        ball.vy = -ball.vy;

        // Add slight random horizontal component to prevent straight bouncing
        if(ball.vx == 0 || abs(ball.vx) < 1) {
          ball.vx = random(0, 2) == 0 ? 1 : -1;
        }
      } else {
        // Bounce horizontally if hit from sides
        ball.vx = -ball.vx;
      }
      
      // Check win condition
      if(blocks_remaining == 0){
        gameover = true;
      }
      
      break; 
    }
  }
}

void ball_pysics(){
  // Move ball
  ball.x += ball.vx;
  ball.y += ball.vy;
  
  // Collisions with left and right walls
  if(ball.x - ball.r <= 0 || ball.x + ball.r >= 64) {
    ball.vx = -ball.vx;
    ball.x = constrain(ball.x, ball.r, 64 - ball.r);
  }
  
  // Collision with top wall
  if(ball.y - ball.r <= 0) {
    ball.vy = -ball.vy;
    ball.y = ball.r;

    // Add slight horizontal variation to prevent vertical bouncing
    if(abs(ball.vx) < 1) {
      ball.vx = random(0, 2) == 0 ? 1 : -1;
    }
  }
  
  // Collision with bottom (game over)
  if(ball.y + ball.r >= 128) {
    gameover = true;
    return;
  }
  
  // Collision with player pedal
  if(ball.y + ball.r >= player_pedal.y && 
     ball.y - ball.r <= player_pedal.y + player_pedal.h &&
     ball.x + ball.r >= player_pedal.x && 
     ball.x - ball.r <= player_pedal.x + player_pedal.w) {
    
    ball.vy = -abs(ball.vy); // Always bounce up
    ball.y = player_pedal.y - ball.r;
    
    // Add spin
    int hitPos = ball.x - (player_pedal.x + player_pedal.w / 2);
    ball.vx = constrain(hitPos / 2, -3, 3);
    
    if(ball.vx == 0) {
      ball.vx = random(0, 2) == 0 ? 1 : -1;
    }
  }
  
  // Check collisions with blocks
  check_block_collisions();
}

void handle_pedal_controls(){
  if(digitalRead(BTN_LEFT) == LOW){
    if (player_pedal.x <= 0) return;
    player_pedal.x -= PEDAL_SPEED;
  }
  if(digitalRead(BTN_RIGHT) == LOW){
    if (player_pedal.x >= 64 - player_pedal.w) return;
    player_pedal.x += PEDAL_SPEED;
  }
}

void draw_game(){
  u8g2.clearBuffer();
  
  handle_pedal_controls();
  ball_pysics();
  
  // Draw blocks
  for(int i = 0; i < MAX_BLOCKS; i++){
    if(blocks[i].active){
      u8g2.drawBox(blocks[i].x, blocks[i].y, blocks[i].w, blocks[i].h);
    }
  }
  
  // Draw ball
  u8g2.drawFilledEllipse(ball.x, ball.y, ball.r, ball.r);
  
  // Draw player pedal 
  u8g2.drawBox(player_pedal.x, player_pedal.y, player_pedal.w, player_pedal.h);
    
  u8g2.sendBuffer();
}

void draw_gameover(){
  u8g2.clearBuffer();
  
  // Check if won or lost
  bool won = (blocks_remaining == 0);
  
  if(won){
    u8g2.setFont(u8g2_font_7x13B_mf);
    u8g2.drawStr(4, 20, "YOU WIN!");
  } else {
    u8g2.setFont(u8g2_font_7x13_mf);
    u8g2.drawStr(1, 20, "Game Over");
  }
    
  // Draw restart prompt
  u8g2.setFont(u8g2_font_5x7_mf);
  u8g2.drawStr(6, 65, "Press any");
  u8g2.drawStr(4, 74, "button to");
  u8g2.drawStr(10, 83, "restart!");
  
  u8g2.sendBuffer();
}