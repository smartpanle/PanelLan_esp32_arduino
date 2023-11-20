#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

void setup(void) {
  tft.begin();

  tft.setTextDatum(textdatum_t::middle_center);
  tft.setFont(&fonts::FreeSerifBold18pt7b);
  tft.drawString("HELLO WORLD", tft.width() / 2, tft.height() / 2);
}

void loop() {
  delay(20);
}
