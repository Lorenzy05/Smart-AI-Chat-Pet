#ifndef oled_display
#define oled_display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_display(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  Serial.println("Succesfully Connected to OLED");

  display.display();
  delay(2000);
  display.clearDisplay();
}

// 简单笑脸
void Face_Smile(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE); // 脸
  display.fillCircle(52, 28, 4, SSD1306_WHITE); // 左眼
  display.fillCircle(76, 28, 4, SSD1306_WHITE); // 右眼
  // 微笑嘴巴（用多条短线模拟弧线）
  for (int i = -10; i <= 10; i++) {
    int x0 = 64 + i;
    int y0 = 48 - (int)(0.08 * i * i);
    display.drawPixel(x0, y0, SSD1306_WHITE);
  }
  display.display();
}

// 普通脸
void Face_Normal(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(52, 28, 4, SSD1306_WHITE);
  display.fillCircle(76, 28, 4, SSD1306_WHITE);
  display.drawLine(54, 48, 74, 48, SSD1306_WHITE); // 平嘴
  display.display();
}

// 哭脸
void Face_Cry(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(52, 28, 4, SSD1306_WHITE);
  display.fillCircle(76, 28, 4, SSD1306_WHITE);
  // 哭嘴（下弯，用多条短线模拟弧线）
  for (int i = -10; i <= 10; i++) {
    int x0 = 64 + i;
    int y0 = 54 + (int)(0.08 * i * i);
    display.drawPixel(x0, y0, SSD1306_WHITE);
  }
  // 泪水
  display.drawLine(52, 32, 52, 40, SSD1306_WHITE);
  display.drawLine(76, 32, 76, 40, SSD1306_WHITE);
  display.display();
}

// 悲伤脸
void Face_Sad(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(52, 28, 4, SSD1306_WHITE);
  display.fillCircle(76, 28, 4, SSD1306_WHITE);
  // 下弯嘴（用多条短线模拟弧线）
  for (int i = -10; i <= 10; i++) {
    int x0 = 64 + i;
    int y0 = 54 + (int)(0.08 * i * i);
    display.drawPixel(x0, y0, SSD1306_WHITE);
  }
  display.display();
}

// 思考中
void Face_Thinking(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(52, 28, 4, SSD1306_WHITE);
  display.fillCircle(76, 28, 4, SSD1306_WHITE);
  display.drawLine(54, 48, 74, 48, SSD1306_WHITE);

  // 思考泡泡
  display.drawCircle(90, 10, 2, SSD1306_WHITE);
  display.drawCircle(100, 6, 2, SSD1306_WHITE);
  display.drawCircle(110, 10, 2, SSD1306_WHITE);

  display.display();
}

// 正在说话（斜线版）
void Face_Speaking(){
  display.clearDisplay();
  display.drawCircle(64, 32, 30, SSD1306_WHITE);
  display.fillCircle(52, 28, 4, SSD1306_WHITE);
  display.fillCircle(76, 28, 4, SSD1306_WHITE);
  display.drawLine(54, 48, 74, 48, SSD1306_WHITE);

  // 声音波
  display.drawLine(74, 48, 80, 44, SSD1306_WHITE);
  display.drawLine(76, 48, 82, 42, SSD1306_WHITE);
  display.drawLine(78, 48, 84, 40, SSD1306_WHITE);

  display.display();
}

#endif