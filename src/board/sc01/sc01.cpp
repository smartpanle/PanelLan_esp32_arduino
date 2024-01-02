#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32

#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include "../boards.h"
#include "sc01_pin.h"

using namespace lgfx::v1;

#define PanelLan_SCREEN_WIDTH 320
#define PanelLan_SCREEN_HIGHT 480
#define PanelLan_SCREEN_BK_FREQ 12000

Panel_Device* panel_load_from_sc01(board_pins_t* pins) {
  auto panle = new lgfx::Panel_ST7796();

  *pins = {
    BOARD_RESET_PIN, BOARD_I2C_SDA_PIN, BOARD_I2C_SCL_PIN,
    { TP_I2C_SDA_PIN, TP_I2C_SCL_PIN, TP_I2C_INT_PIN, TP_I2C_RST_PIN },
    { -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1 },
    { -1, -1 },
    { -1, -1, -1, -1, -1, -1 }
  };

  {
    auto _bus_instance = new Bus_SPI();
    auto cfg     =  _bus_instance->config();
    cfg.freq_write = 40000000;
    cfg.pin_sclk =  LCD_CLK_PIN;
    cfg.pin_mosi =  LCD_MOSI_PIN;
    cfg.pin_dc =    LCD_DC_PIN;
    cfg.pin_miso =  LCD_MISO_PIN;
    cfg.spi_3wire = true;
    _bus_instance->config(cfg);
    panle->setBus(_bus_instance);
  }

  {
    auto cfg = panle->config(); //
    cfg.pin_cs      = LCD_CS_PIN;
    cfg.pin_rst     = LCD_RST_PIN;
    cfg.readable    = false;
    cfg.bus_shared  = false;
    panle->config(cfg);
  }

  {
    auto _touch_instance = new Touch_FT5x06();
    auto cfg             = _touch_instance->config();
    cfg.x_max            = PanelLan_SCREEN_WIDTH - 1;
    cfg.y_max            = PanelLan_SCREEN_HIGHT - 1;
    cfg.bus_shared       = false;
    cfg.offset_rotation  = 0;

    cfg.i2c_port = I2C_NUM_1;

    cfg.pin_int = TP_I2C_INT_PIN;
    cfg.pin_sda = TP_I2C_SDA_PIN;
    cfg.pin_scl = TP_I2C_SCL_PIN;
    cfg.pin_rst = TP_I2C_RST_PIN;

    cfg.freq = 400000;
    _touch_instance->config(cfg);
    panle->setTouch(_touch_instance);
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
