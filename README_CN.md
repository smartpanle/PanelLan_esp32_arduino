# 描述

本仓库提供对启明智显所销售的开发板的屏幕和触摸功能的支持，使开发过程更加迅速和高效。

# 如何使用

支持的开发板已经配置好, 可直接点亮使用 

```c++
#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PANLEE tft(BOARD_SC01_PLUS);

void setup(void)
{
  tft.begin();
  
  tft.setTextDatum(textdatum_t::middle_center);
  tft.setFont(&fonts::FreeSerifBold18pt7b);
  tft.drawString("HELLO WORLD",tft.width() / 2,  tft.height() / 2);
}
```

仓库屏幕及触摸驱动部分依赖于[LovyanGFX](https://github.com/lovyan03/LovyanGFX), 示例可以参照[这里](https://github.com/lovyan03/LovyanGFX/tree/master/examples).

# 支持的开发板

||||||
|---|---|---|---|---|
|名称|型号|LCD接口|触摸|其它外设|
|SC01|-|spi/ST7796|FT5x06|-|
|SC01-PLUS|ZX3D50CE08S|8080-8bit / ST7796|FT5x06|sdcard, spk, rs485|
|SC02|ZX4D30NE01S-UR|RGB|-|rs485|
|SC05|ZX7D00CE01S|RGB|GT911|-|
|KC01|ZX2D10GE01S|RGB|-|ab-encoder, motor|
|BC02|ZX3D95CE01S-TR|RGB|FT5x06|sht20|


# Board 选择配置

|||||
|---|---|---|---|
|名称|型号|FLSAH|PSRAM|
|SC01|ESP32 Dev Module|4MB QIO 80MHz|QSPI PSRAM|
|SC01-PLUS|ESP32S3 Dev Module|8MB QIO 80MHz|QSPI PSRAM|
|SC02|ESP32S3 Dev Module|16MB QIO 80MHz|OPI PSRAM|
|SC05|ESP32S3 Dev Module|16MB QIO 80MHz|OPI PSRAM|
|KC01|ESP32S3 Dev Module|16MB QIO 80MHz|OPI PSRAM|
|BC02|ESP32S3 Dev Module|16MB QIO 80MHz|OPI PSRAM|


# 依赖库

|名称|版本|介绍|
|---|---|---|
|  [LovyanGFX](https://github.com/lovyan03/LovyanGFX)  | >= v1.1.9|SPI LCD graphics library for ESP32  / ESP8266  / SAMD51|
|  [arduino-esp32](https://github.com/espressif/arduino-esp32)  |  >= v2.0.9|Arduino core for the ESP32, ESP32-S3, etc|


# 感谢

- 感谢   [lovyan03](https://github.com/lovyan03)  开源了优秀好用且易于扩展的的驱动仓库  [LovyanGFX](https://github.com/lovyan03/LovyanGFX)  

