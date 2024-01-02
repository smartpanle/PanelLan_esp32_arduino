#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

#include <driver/i2c.h>
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include "../boards.h"
#include "kc01_pin.h"
#include "kc01_rgb_spi_init.hpp"

using namespace lgfx::v1;

#define PanelLan_SCREEN_WIDTH 480
#define PanelLan_SCREEN_HIGHT 480

#define PanelLan_SCREEN_BK_FREQ     22222

#define PanelLan_RGB_CLK_FREQ           (15000000)
#define PanelLan_RGB_HSYNC_PULSE_WIDTH  (10)
#define PanelLan_RGB_HSYNC_BACK_PORCH   (10)
#define PanelLan_RGB_HSYNC_FRONT_PORCH  (10)
#define PanelLan_RGB_VSYNC_PULSE_WIDTH  (2)
#define PanelLan_RGB_VSYNC_BACK_PORCH   (12)
#define PanelLan_RGB_VSYNC_FRONT_PORCH  (14)

class Panel_KC01_Spec: public Panel_RGB {
  bool init(bool use_reset) override {
    if (BOARD_RESET_PIN > -1) {
      pinMode(BOARD_RESET_PIN, OUTPUT);
      digitalWrite(BOARD_RESET_PIN, 0);
      vTaskDelay(pdMS_TO_TICKS(2));
      digitalWrite(BOARD_RESET_PIN, 1);
      vTaskDelay(pdMS_TO_TICKS(120));
    }

    if (LCD_DISP_EN_GPIO > -1) {
      pinMode(LCD_DISP_EN_GPIO, OUTPUT);
      digitalWrite(LCD_DISP_EN_GPIO, 1);
      vTaskDelay(pdMS_TO_TICKS(10));
    }

    panelLan_rgb_spi_init();

    if (!Panel_RGB::init(false)) {
      return false;
    }

    return true;
  }
};

Panel_Device* panel_load_from_kc01(board_pins_t* pins) {
  auto panle = new Panel_KC01_Spec();

  *pins = {
    BOARD_RESET_PIN, BOARD_I2C_SDA_PIN, BOARD_I2C_SCL_PIN,
    { TP_I2C_SDA_PIN, TP_I2C_SCL_PIN, TP_I2C_INT_PIN, TP_I2C_RST_PIN },
    { I2S_MCLK_PIN, I2S_SCLK_PIN, I2S_LRCK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN },
    { SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN, SD_D1_PIN, SD_D2_PIN, SD_D3_PIN },
    { RS485_RTS_PIN, RS485_RXD_PIN, RS485_TXD_PIN },
    { USB_DP_PIN, USB_DN_PIN },
    { EXTERNAL_PIN_1, EXTERNAL_PIN_2, EXTERNAL_PIN_3, EXTERNAL_PIN_4, -1, -1 }
  };

  {
    auto cfg = panle->config_detail();
    // cfg.use_psram = 1;
    panle->config_detail(cfg);
  }

  {
    auto _bus_instance = new Bus_RGB();
    auto cfg           = _bus_instance->config();
    cfg.panel          = panle;
    cfg.pin_d0         = LCD_D0_GPIO;  // B0
    cfg.pin_d1         = LCD_D1_GPIO;  // B1
    cfg.pin_d2         = LCD_D2_GPIO;  // B2
    cfg.pin_d3         = LCD_D3_GPIO;  // B3
    cfg.pin_d4         = LCD_D4_GPIO;  // B4
    cfg.pin_d5         = LCD_D5_GPIO;  // G0
    cfg.pin_d6         = LCD_D6_GPIO;  // G1
    cfg.pin_d7         = LCD_D7_GPIO;  // G2
    cfg.pin_d8         = LCD_D8_GPIO;  // G3
    cfg.pin_d9         = LCD_D9_GPIO;  // G4
    cfg.pin_d10        = LCD_D10_GPIO; // G5
    cfg.pin_d11        = LCD_D11_GPIO; // R0
    cfg.pin_d12        = LCD_D12_GPIO; // R1
    cfg.pin_d13        = LCD_D13_GPIO; // R2
    cfg.pin_d14        = LCD_D14_GPIO; // R3
    cfg.pin_d15        = LCD_D15_GPIO; // R4

    cfg.pin_pclk    = LCD_PCLK_GPIO;
    cfg.pin_vsync   = LCD_VSYNC_GPIO;
    cfg.pin_hsync   = LCD_HSYNC_GPIO;
    cfg.pin_henable = LCD_DE_GPIO;

    cfg.freq_write = PanelLan_RGB_CLK_FREQ;

    cfg.hsync_polarity    = 1;
    cfg.hsync_front_porch = PanelLan_RGB_HSYNC_FRONT_PORCH;
    cfg.hsync_pulse_width = PanelLan_RGB_HSYNC_PULSE_WIDTH;
    cfg.hsync_back_porch  = PanelLan_RGB_HSYNC_BACK_PORCH;
    cfg.vsync_polarity    = 1;
    cfg.vsync_front_porch = PanelLan_RGB_VSYNC_FRONT_PORCH;
    cfg.vsync_pulse_width = PanelLan_RGB_VSYNC_PULSE_WIDTH;
    cfg.vsync_back_porch  = PanelLan_RGB_VSYNC_BACK_PORCH;
    cfg.pclk_active_neg   = 0;
    _bus_instance->config(cfg);
    panle->setBus(_bus_instance);
  }

  {
    auto cfg = panle->config(); //

    cfg.memory_width  = PanelLan_SCREEN_WIDTH;
    cfg.panel_width   = PanelLan_SCREEN_WIDTH;
    cfg.memory_height = PanelLan_SCREEN_HIGHT;
    cfg.panel_height  = PanelLan_SCREEN_HIGHT;

    cfg.offset_x = 0;
    cfg.offset_y = 0;

    panle->config(cfg);
  }

  {
    auto blk = new Light_PWM();
    auto cfg = blk->config();

    cfg.pin_bl      = LCD_BL_PIN;
    cfg.invert      = false;
    cfg.freq        = PanelLan_SCREEN_BK_FREQ;
    cfg.pwm_channel = 7;

    blk->config(cfg);
    panle->setLight(blk);
  }

  return panle;
}

#endif
