#include "PanelLan.h"
#include <Adafruit_AW9523.h>

// BOARD_SC05
PanelLan tft(BOARD_SC05);

#define AW9523_I2C_ADDR 0x5B
#define I2C_SDA_PIN 48
#define I2C_SCL_PIN 47

Adafruit_AW9523 aw;
uint8_t LedPin = 0;
uint8_t LCD_RST_Pin = 8;  // aw9523 P10 connected to LCD_RST
uint8_t TP_RST_Pin = 9;   // aw9523 P11 connected to TP_RST

void aw9523_test() {
  Serial.begin(115200);
  while (!Serial)
    delay(1);

  Serial.println("Adafruit AW9523 GPIO Expander test!");

  Wire1.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire1.setClock(400000);

  // Initialize the AW9523 GPIO expander
  if (!aw.begin(AW9523_I2C_ADDR, &Wire1))
  {
    Serial.println("AW9523 not found? Check wiring!");
    while (1)
      delay(10);
  }
  Serial.println("AW9523 found!");

  aw.pinMode(LCD_RST_Pin, OUTPUT);
  aw.pinMode(TP_RST_Pin, OUTPUT);
  aw.digitalWrite(LCD_RST_Pin, HIGH);
  aw.digitalWrite(TP_RST_Pin, HIGH);

  aw.pinMode(LedPin, OUTPUT);
 
}

void setup(void) {
  aw9523_test();
}

void loop() {
  aw.digitalWrite(LedPin, HIGH);
  delay(100);
  aw.digitalWrite(LedPin, LOW);
  delay(100);
}
