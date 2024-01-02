#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include "../boards.h"
#include "sc01_plus_pin.h"

using namespace lgfx::v1;

#define PanelLan_SCREEN_WIDTH 320
#define PanelLan_SCREEN_HIGHT 480
#define PanelLan_SCREEN_BK_FREQ 21111
#define PanelLan_SCREEN_CLK_SPEED (40 * 1000 * 1000)

class Panel_SC01PLUS_Spec: public Panel_ST7796 {
  protected:
    const uint8_t* getInitCommands(uint8_t listno) const override {
      static constexpr uint8_t list0[] = {
        0x11, 0+CMD_INIT_DELAY, 120,    // Exit sleep mode

        0x36, 1, 0x48,
        0x3A, 1, 0x55,
        0xf0, 1, 0xc3,
        0xf0, 1, 0x96,
        0xb4, 1, 0x01,
        0xb5, 1, 0x1e,
        0xb6, 3, 0x80, 0x22, 0x3b,
        0xb7, 1, 0xc6,
        0xb9, 2, 0x02, 0xe0,
        0xc0, 2, 0x80, 0x16,
        0xc1, 1, 0x19,
        0xc2, 1, 0xa7,
        0xc5, 1, 0x16,
        0xE8, 8, 0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33,   
        0xe0, 14, 0xF0, 0x07, 0x0D, 0x04, 0x05, 0x14, 0x36, 0x54, 0x4C, 0x38, 0x13, 0x14, 0x2E, 0x34,   
        0xe1, 14, 0xF0, 0x10, 0x14, 0x0E, 0x0C, 0x08, 0x35, 0x44, 0x4C, 0x26, 0x10, 0x12, 0x2C, 0x32, 
        0xf0, 1, 0x3c,
        0xf0, 1 + CMD_INIT_DELAY, 0x69, 120,

        0x29, 0,
        0x21, 0,
        0xFF,0xFF, // end
      };

      switch (listno) {
        case 0: return list0;
        default: return nullptr;
      }
    }
};

Panel_Device* panel_load_from_sc01_plus(board_pins_t* pins) {
  auto panle = new Panel_SC01PLUS_Spec();

  *pins = {
    BOARD_RESET_PIN, BOARD_I2C_SDA_PIN, BOARD_I2C_SCL_PIN,
    { TP_I2C_SDA_PIN, TP_I2C_SCL_PIN, TP_I2C_INT_PIN, TP_I2C_RST_PIN },
    { I2S_MCLK_PIN, I2S_SCLK_PIN, I2S_LRCK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN },
    { SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN, SD_D1_PIN, SD_D2_PIN, SD_D3_PIN },
    { RS485_RTS_PIN, RS485_RXD_PIN, RS485_TXD_PIN },
    { USB_DP_PIN, USB_DN_PIN },
    { EXTERNAL_PIN_1, EXTERNAL_PIN_2, EXTERNAL_PIN_3, EXTERNAL_PIN_4, EXTERNAL_PIN_5, EXTERNAL_PIN_6 }
  };

  {
    auto _bus_instance = new Bus_Parallel8();
    auto cfg           = _bus_instance->config();
    cfg.port           = 0;
    cfg.freq_write     = PanelLan_SCREEN_CLK_SPEED;
    cfg.pin_wr         = LCD_WR_PIN; // WR
    cfg.pin_rd         = LCD_RD_PIN; // RD
    cfg.pin_rs         = LCD_RS_PIN; // RS(D/C)
    cfg.pin_d0         = LCD_D0_PIN; // D0
    cfg.pin_d1         = LCD_D1_PIN; // D1
    cfg.pin_d2         = LCD_D2_PIN; // D2
    cfg.pin_d3         = LCD_D3_PIN; // D3
    cfg.pin_d4         = LCD_D4_PIN; // D4
    cfg.pin_d5         = LCD_D5_PIN; // D5
    cfg.pin_d6         = LCD_D6_PIN; // D6
    cfg.pin_d7         = LCD_D7_PIN; // D7
    _bus_instance->config(cfg);
    panle->setBus(_bus_instance);
  }

  {
    auto cfg = panle->config(); //

    cfg.pin_cs   = -1;
    cfg.pin_rst  = BOARD_RESET_PIN;
    cfg.pin_busy = -1;

    cfg.memory_width     = PanelLan_SCREEN_WIDTH;
    cfg.memory_height    = PanelLan_SCREEN_HIGHT;
    cfg.panel_width      = PanelLan_SCREEN_WIDTH;
    cfg.panel_height     = PanelLan_SCREEN_HIGHT;
    cfg.offset_x         = 0;
    cfg.offset_y         = 0;
    cfg.offset_rotation  = 2;
    cfg.dummy_read_pixel = 8;
    cfg.dummy_read_bits  = 1;
    cfg.readable         = false;
    cfg.invert           = true;
    cfg.rgb_order        = false;
    cfg.dlen_16bit       = false;
    cfg.bus_shared       = false;
    panle->config(cfg);
  }

  {
    auto _touch_instance = new Touch_FT5x06();
    auto cfg             = _touch_instance->config();
    cfg.x_min            = 0;
    cfg.x_max            = PanelLan_SCREEN_WIDTH;
    cfg.y_min            = 0;
    cfg.y_max            = PanelLan_SCREEN_HIGHT;
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