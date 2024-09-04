#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include "../boards.h"
#include "sc08_pin.h"
#include "Panel_NV3041A.hpp"
#include "Touch_CST3240.hpp"

using namespace lgfx::v1;

#define PANLEE_SCREEN_WIDTH 480
#define PANLEE_SCREEN_HIGHT 272
#define PANLEE_SCREEN_BK_FREQ 11111
#define PANLEE_SCREEN_CLK_SPEED (4 * 1000 * 1000)

class Panel_NV3041A_Spec: public Panel_NV3041A {
  protected:
    const uint8_t* getInitCommands(uint8_t listno) const override {
      static constexpr uint8_t list0[] = {
        0xFF, 1, 0xA5,
        0xE7, 1, 0x10,
        0x35, 1, 0x01,
        0x3A, 1, 0x01,
        0x40, 1, 0x01,
        0x41, 1, 0x01,
        0x55, 1, 0x01,
        0x44, 1, 0x15,
        0x45, 1, 0x15,
        0x7D, 1, 0x03,
        0xC1, 1, 0xAB,
        0xC2, 1, 0x17,
        0xC3, 1, 0x10,
        0xC6, 1, 0x3A,
        0xC7, 1, 0x25,
        0xC8, 1, 0x11,
        0x7A, 1, 0x49,
        0x6F, 1, 0x2F,
        0x78, 1, 0x4B,
        0x73, 1, 0x08,
        0x74, 1, 0x12,
        0xC9, 1, 0x00,
        0x67, 1, 0x11,
        0x51, 1, 0x20,
        0x52, 1, 0x7C,
        0x53, 1, 0x1C,
        0x54, 1, 0x77,
        0x46, 1, 0x0A,
        0x47, 1, 0x2A,
        0x48, 1, 0x0A,
        0x49, 1, 0x1A,
        0x56, 1, 0x43,
        0x57, 1, 0x42,
        0x58, 1, 0x3C,
        0x59, 1, 0x64,
        0x5A, 1, 0x41,
        0x5B, 1, 0x3C,
        0x5C, 1, 0x02,
        0x5D, 1, 0x3C,
        0x5E, 1, 0x1F,
        0x60, 1, 0x80,
        0x61, 1, 0x3F,
        0x62, 1, 0x21,
        0x63, 1, 0x07,
        0x64, 1, 0xE0,
        0x65, 1, 0x01,
        0xCA, 1, 0x20,
        0xCB, 1, 0x52,
        0xCC, 1, 0x10,
        0xCD, 1, 0x42,
        0xD0, 1, 0x20,
        0xD1, 1, 0x52,
        0xD2, 1, 0x10,
        0xD3, 1, 0x42,
        0xD4, 1, 0x0A,
        0xD5, 1, 0x32,
        0xE5, 1, 0x06,
        0xE6, 1, 0x00,
        0x6E, 1, 0x14,
        0x80, 1, 0x04,
        0xA0, 1, 0x00,
        0x81, 1, 0x07,
        0xA1, 1, 0x05,
        0x82, 1, 0x06,
        0xA2, 1, 0x04,
        0x83, 1, 0x39,
        0xA3, 1, 0x39,
        0x84, 1, 0x3A,
        0xA4, 1, 0x3A,
        0x85, 1, 0x3F,
        0xA5, 1, 0x3F,
        0x86, 1, 0x30,
        0xA6, 1, 0x30,
        0x87, 1, 0x3A,
        0xA7, 1, 0x3A,
        0x88, 1, 0x08,
        0xA8, 1, 0x08,
        0x89, 1, 0x0F,
        0xA9, 1, 0x0F,
        0x8A, 1, 0x17,
        0xAA, 1, 0x17,
        0x8B, 1, 0x10,
        0xAB, 1, 0x10,
        0x8C, 1, 0x16,
        0xAC, 1, 0x16,
        0x8D, 1, 0x14,
        0xAD, 1, 0x14,
        0x8E, 1, 0x11,
        0xAE, 1, 0x11,
        0x8F, 1, 0x14,
        0xAF, 1, 0x14,
        0x90, 1, 0x06,
        0xB0, 1, 0x06,
        0x91, 1, 0x0F,
        0xB1, 1, 0x0F,
        0x92, 1, 0x16,
        0xB2, 1, 0x16,
        0xFF, 1, 0x00,
        0x11, 0+CMD_INIT_DELAY, 120,    // Exit sleep mode

        0x29, 0+CMD_INIT_DELAY, 120,
        0xFF,0xFF, // end
      };

      switch (listno) {
        case 0: return list0;
        default: return nullptr;
      }
    }
};

Panel_Device* panel_load_from_sc08(board_pins_t* pins) {
  auto panle = new Panel_NV3041A_Spec();

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
    auto _bus_instance = new Bus_Parallel8();
    auto cfg           = _bus_instance->config();
    cfg.port           = 0;
    cfg.freq_write     = PANLEE_SCREEN_CLK_SPEED;
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

    cfg.memory_width     = PANLEE_SCREEN_WIDTH;
    cfg.memory_height    = PANLEE_SCREEN_HIGHT;
    cfg.panel_width      = PANLEE_SCREEN_WIDTH;
    cfg.panel_height     = PANLEE_SCREEN_HIGHT;
    cfg.offset_x         = 0;
    cfg.offset_y         = 0;
    cfg.offset_rotation  = 0;
    cfg.dummy_read_pixel = 8;
    cfg.dummy_read_bits  = 1;
    cfg.readable         = false;
    cfg.invert           = false;
    cfg.rgb_order        = true;
    cfg.dlen_16bit       = false;
    cfg.bus_shared       = false;
    panle->config(cfg);
  }

  {
    auto _touch_instance = new Touch_CST3240();
    auto cfg             = _touch_instance->config();
    cfg.x_min            = PANLEE_SCREEN_WIDTH;
    cfg.x_max            = 0;
    cfg.y_min            = PANLEE_SCREEN_HIGHT;
    cfg.y_max            = 0;
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
    cfg.freq        = PANLEE_SCREEN_BK_FREQ;
    cfg.pwm_channel = 7;

    blk->config(cfg);
    panle->setLight(blk);
  }

  return panle;
}
