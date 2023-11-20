#include "PanelLan.h"
#include "driver/uart.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

#define BUF_SIZE (127)

void setup(void) {
  Serial.begin(115200);

  tft.begin();
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(0xffffffU);
  tft.setTextWrap(true, true);

  if ((tft.pins.rs485.rxd == -1) || (tft.pins.rs485.txd == -1) || (tft.pins.rs485.rts == -1)) {
    tft.println("The current board does not support rs485");
    Serial.printf("The current board does not support rs485\n");
    return;
  }

  // Set rs485
  Serial2.begin(115200);

  // Set UART pins as per KConfig settings
  Serial2.setPins(tft.pins.rs485.rxd, tft.pins.rs485.txd, -1, tft.pins.rs485.rts);

  // Set RS485 half duplex mode
  Serial2.setMode(UART_MODE_RS485_HALF_DUPLEX);

  // Sets the maximum number of milliseconds to wait
  Serial2.setTimeout(3);

  Serial.printf("Start RS485 UART TEST.\r\n");
  tft.println("Start RS485 UART TEST...\r\n");
  Serial2.write("Start RS485 UART TEST.\r\n");
}

void loop(void) {
  // Allocate buffers for UART
  uint8_t* data = (uint8_t*)malloc(BUF_SIZE);

  while (1) {
    // Read data from UART
    int len = Serial2.read(data, BUF_SIZE);

    // Write data back to UART
    if (len > 0) {
      Serial2.write("\r\n");
      tft.printf("\r\n");
      char prefix[] = "RS485 Received: [";
      Serial2.write(prefix, (sizeof(prefix) - 1));
      tft.printf(prefix);
      Serial.printf("[");
      for (int i = 0; i < len; i++) {
        Serial.printf("0x%x", (uint8_t)data[i]);
        Serial2.write((const char*)&data[i], 1);
        tft.printf("0x%x", (uint8_t)data[i]);
        // Add a Newline character if you get a return charater from paste (Paste tests multibyte receipt/buffer)
        if (data[i] == '\r') {
          Serial2.write("\n", 1);
        }
      }
      Serial.printf("] \n");
      Serial2.write("]\r\n", 3);
      tft.printf("] \n");
    } else {
      // Echo a "." to show we are alive while we wait for input
      Serial2.write(".");
      //tft.printf(".");
      delay(10);
    }
  }
  delay(1000);
}