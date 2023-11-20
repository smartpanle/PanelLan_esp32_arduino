#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

void setup(void) {
  Serial.begin(115200);

  tft.begin();
  tft.fillScreen(TFT_WHITE);
  tft.drawString("Font test", 10, 10);

  // text color
  tft.setTextColor(0x000000U);

  // change font
  tft.setFont(&fonts::Font2);
  tft.setCursor(10, 30);
  tft.println("Font2:\n Hello World");

  tft.setFont(&fonts::Font4);
  tft.setCursor(10, 60);
  tft.println("Font4:\n Hello World");

  tft.setFont(&fonts::FreeSerif12pt7b);
  tft.setCursor(10, 120);
  tft.println("FreeSerif24pt7b:\n Hello World");

  tft.setFont(&fonts::FreeSerifBold12pt7b);
  tft.setCursor(10, 180);
  tft.println("FreeSerifBold24pt7b:\n Hello World");
}

void loop() {
  delay(1000);
}
