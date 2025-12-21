#include <U8g2lib.h>

// ===== HARDWARE CONFIGURATION =====
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, -1, A5, A4);

#define BTN_RIGHT 10
#define BTN_LEFT 7

struct pedal{
  int x;
  int y;
  bool active;    
};

// forward declaration
void draw_game();
void initialize_game();

void setup() {
  u8g2.begin();
  
  // intialising buttons 
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);

}

void loop() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_7x13_mf);
  u8g2.drawStr(6, 32, "hi");

  u8g2.sendBuffer();
}
