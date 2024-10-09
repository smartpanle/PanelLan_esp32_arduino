#include "PanelLan.h"
#include "AW9523B.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC05);

#define I2C_SDA 48
#define I2C_SCL 47

AW9523B aw9523(I2C_SDA, I2C_SCL, 0, 0);

void aw9523_test(){
    // aw9523.begin(); // Initialize AW9523B

    uint8_t id = aw9523.readID();
    Serial.print("[AW9523B] ID: 0x");
    Serial.println(id, HEX);

    // Set all ports to output mode
    aw9523.setConfig(0, 0x00); // Port 0 config
    // aw9523.setConfig(1, 0x00); // Port 1 config

    // // Set P00, P01, P02 high
    aw9523.writePort(0, 0x07); // 0b00000111 (P00, P01, P02 high)
    
    // aw9523.writePort(1, 0x00); 
}

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.initDMA();
  tft.fillScreen(TFT_GREEN);
  tft.drawString("touch test", 10, 10);

  Serial.printf("Fill screen finish\r\n");
  aw9523_test();
}

void loop() {
  int16_t x = 0;
  int16_t y = 0;

  if (tft.getTouch(&x, &y) > 0) {
    tft.drawPixel(x, y, TFT_RED);
  }

  delay(20);
}
