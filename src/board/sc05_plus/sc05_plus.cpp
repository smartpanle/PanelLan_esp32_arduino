#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include "../boards.h"
#include "sc05_plus_pin.h"

using namespace lgfx::v1;

#define PanelLan_SCREEN_WIDTH 240
#define PanelLan_SCREEN_HIGHT 320
#define PanelLan_SCREEN_BK_FREQ 21111
#define PanelLan_SCREEN_CLK_SPEED (20 * 1000 * 1000)

class Panel_SC05PLUS_Spec: public Panel_ST7789 {
  protected:
    const uint8_t* getInitCommands(uint8_t listno) const override {
      static constexpr uint8_t list0[] = {
        0x11, 0+CMD_INIT_DELAY, 120,    // Exit sleep mode
        0x36, 1, 0x00,
        0x3A, 1, 0x05,
        0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33,
        0xB7, 1, 0x46,
        0xBB, 1, 0x1B,
        0xC0, 1, 0x2C,
        0xC2, 1, 0x01,
        0xC3, 1, 0x0F,
        0xC4, 1, 0x20,
        0xC6, 1, 0x0F,
        0xD0, 2, 0xA4, 0xA1,
        0xD6, 1, 0xA1,
        0xE0, 14, 0xF0, 0x00, 0x06, 0x04, 0x05, 0x05, 0x31, 0x44, 0x48, 0x36, 0x12, 0x12, 0x2B, 0x34,
        0xE1, 14, 0xF0, 0x0B, 0x0F, 0x0F, 0x0D, 0x26, 0x31, 0x43, 0x47, 0x38, 0x14, 0x14, 0x2C, 0x32,
        0x21, 0,
        0x29, 0,
        0x2C, 0,
        0xFF,0xFF, // end
      };

      switch (listno) {
        case 0: return list0;
        default: return nullptr;
      }
    }
};

class Touch_GT911_Spec : public ITouch {

public:
  bool init(void) {
    lgfx::i2c::init(_cfg.i2c_port, _cfg.pin_sda, _cfg.pin_scl);
    if (_cfg.pin_int >= 0) {
      lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::input);
    }
    return true;
  }

  void wakeup(void) {

  }

  void sleep(void) {

  }

  uint_fast8_t getTouchRaw(touch_point_t *tp, uint_fast8_t count) override {
    if ((_cfg.pin_int < 0 || !gpio_in(_cfg.pin_int))) {
      if (this->_update_data()) {
        tp->x = ((_readdata[1] & 0x0f) << 8) | _readdata[0];
        tp->y = ((_readdata[3] & 0x0f) << 8) | _readdata[2];
        return 1;
      }
    }
    return 0;
  }

private:
  enum
  {
    max_touch_points = 1
  };
  uint8_t _readdata[max_touch_points * 8 + 2]; // 5point * 8byte + 2byte

  bool _writeBytes(const uint8_t* data, size_t len) {
    return lgfx::i2c::transactionWrite(_cfg.i2c_port, _cfg.i2c_addr, data, len, _cfg.freq).has_value();
  }

  bool _writeReadBytes(const uint8_t* write_data, size_t write_len, uint8_t* read_data, size_t read_len) {
    return lgfx::i2c::transactionWriteRead(_cfg.i2c_port, _cfg.i2c_addr, write_data, write_len, read_data, read_len, _cfg.freq).has_value();
  }

  bool _update_data(void) {
    uint8_t gt911cmd_getdata[] = { 0x81, 0x4E, 0x00 };
    _writeReadBytes(gt911cmd_getdata, 2, &_readdata[0], 1);
    if ((_readdata[0] & 0x80) || (_readdata[0] & 0x0f)) {
      _writeBytes(gt911cmd_getdata, 3);
    }

    if ((_readdata[0] & 0x0f) == 0) {
      return false;
    }

    uint8_t gt911cmd_getpoint[] = { 0x81, 0x50, 0x00 };
    _writeReadBytes(gt911cmd_getpoint, 2, &_readdata[0], 4);
    return true;
  }
};


Panel_Device* panel_load_from_sc05_plus(board_pins_t* pins) {
  pinMode(BOARD_RESET_PIN, lgfx::pin_mode_t::output);
  pinMode(TP_I2C_INT_PIN, lgfx::pin_mode_t::output);
  gpio_lo(BOARD_RESET_PIN);
  gpio_lo(TP_I2C_INT_PIN);
  lgfx::delay(2);
  gpio_hi(BOARD_RESET_PIN);
  lgfx::delay(50);
  pinMode(TP_I2C_INT_PIN, lgfx::pin_mode_t::input);

  auto panle = new Panel_SC05PLUS_Spec();

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
    cfg.pin_rst  = -1;
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
    auto _touch_instance = new Touch_GT911_Spec();
    auto cfg             = _touch_instance->config();
    cfg.x_min            = 0;
    cfg.x_max            = PanelLan_SCREEN_WIDTH;
    cfg.y_min            = 0;
    cfg.y_max            = PanelLan_SCREEN_HIGHT;
    cfg.bus_shared       = true;
    cfg.offset_rotation  = 0;
    cfg.i2c_port = I2C_NUM_1;
    cfg.pin_int = TP_I2C_INT_PIN;
    cfg.pin_sda = TP_I2C_SDA_PIN;
    cfg.pin_scl = TP_I2C_SCL_PIN;
    cfg.pin_rst = TP_I2C_RST_PIN;
    cfg.i2c_addr = 0x5d;
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