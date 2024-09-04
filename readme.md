# Description

This repository supports screens and touch functions of development boards sold by Qiming Zhixian, making development faster and more efficient.

# How to Use

Supported boards are pre-configured and ready to use.

```c++
#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

void setup(void)
{
  tft.begin();
  tft.setTextDatum(textdatum_t::middle_center);
  tft.setFont(&fonts::FreeSerifBold18pt7b);
  tft.drawString("HELLO WORLD", tft.width() / 2, tft.height() / 2);
}
```

The screen and touch drivers in the repository rely on [LovyanGFX](https://github.com/lovyan03/LovyanGFX). Examples are at [here](https://github.com/lovyan03/LovyanGFX/tree/master/examples) .

# Supported Boards

| Name | Model | LCD Interface | Touch | Other Peripherals |
|------|-------|---------------|-------|--------------------|
| SC01 | - | spi/ST7796 | FT5x06 | - |
| SC01-PLUS | ZX3D50CE08S | 8080-8bit / ST7796 | FT5x06 | sdcard, spk, rs485 |
| SC02 | ZX4D30NE01S-UR | RGB | - | rs485 |
| SC05 | ZX7D00CE01S | RGB | GT911 | - |
| SC07 | ZX4D30CE08S-4827 | 8080 | CST3240 | rs485, can, buzzer|
| KC01 | ZX2D10GE01S | RGB | - | ab-encoder, motor |
| BC02 | ZX3D95CE01S-TR | RGB | FT5x06 | sht20 |

# Board Configuration

| Name | Model | FLASH | PSRAM |
|------|-------|-------|-------|
| SC01 | ESP32 Dev Module | 4MB QIO 80MHz | QSPI PSRAM |
| SC01-PLUS | ESP32S3 Dev Module | 8MB QIO 80MHz | QSPI PSRAM |
| SC02 | ESP32S3 Dev Module | 16MB QIO 80MHz | OPI PSRAM |
| SC05 | ESP32S3 Dev Module | 16MB QIO 80MHz | OPI PSRAM |
| SC07 | ESP32S3 Dev Module | 16MB QIO 80MHz | OPI PSRAM |
| KC01 | ESP32S3 Dev Module | 16MB QIO 80MHz | OPI PSRAM |
| BC02 | ESP32S3 Dev Module | 16MB QIO 80MHz | OPI PSRAM |

# Dependencies

| Name | Version | Description |
|------|---------|-------------|
| [LovyanGFX](https://github.com/lovyan03/LovyanGFX) | >= v1.1.9 | SPI LCD graphics library for ESP32 / ESP8266 / SAMD51 |
| [arduino-esp32](https://github.com/espressif/arduino-esp32) | >= v2.0.9 | Arduino core for the ESP32, ESP32-S3, etc |

# Other
If you encounter screen tearing issues with RGB, refer to the following link:
https://github.com/esp-arduino-libs/arduino-esp32-sdk


# Acknowledgements

- Thanks to [lovyan03](https://github.com/lovyan03) for the open-source, user-friendly, and easily extendable driver repository [LovyanGFX](https://github.com/lovyan03/LovyanGFX).
