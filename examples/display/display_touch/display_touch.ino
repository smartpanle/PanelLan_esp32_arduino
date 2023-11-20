#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.initDMA();
  tft.fillScreen(TFT_GREEN);
  tft.drawString("touch test", 10, 10);

  Serial.printf("Fill screen finish\r\n");
}

void loop() {
  int16_t x = 0;
  int16_t y = 0;

  if (tft.getTouch(&x, &y) > 0) {
    tft.drawPixel(x, y, TFT_RED);
  }

  delay(20);
}
