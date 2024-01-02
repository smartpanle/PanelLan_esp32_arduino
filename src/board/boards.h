#pragma once

#include <LovyanGFX.hpp>

enum panelLan_board_t {
#ifdef CONFIG_IDF_TARGET_ESP32
  BOARD_SC01,             //
#elif CONFIG_IDF_TARGET_ESP32S3
  BOARD_SC01_PLUS = 0x05, // ZX3D50CE08S
  BOARD_SC02,             // ZX4D30NE01S-UR-4827
  BOARD_SC05,             // ZX7D00CE01S
  BOARD_KC01,             // ZX2D10GE01S
  BOARD_BC02,             // ZX3D95CE01S
#endif
};

typedef struct {
  // BOARD BASE PIN
  int8_t reset;
  int8_t i2c_sda;
  int8_t i2c_scl;

  // TOUCH PAD PIN
  struct {
    int8_t i2c_sda, i2c_scl, i2c_int, i2c_rst;
  } touchPad;

  // I2S PIN
  struct {
    int8_t mclk, sclk, lrck, dout, din;
  } i2s;

  // SD CARD
  struct {
    int8_t clk, cmd, d0, d1, d2, d3;
  } sd;

  // UART-485
  struct {
    int8_t rts, rxd, txd;
  } rs485;

  // USB
  struct {
    int8_t dp, dn;
  } usb;

  // EXTERNAL PIN
  struct {
    int8_t pin_1, pin_2, pin_3, pin_4, pin_5, pin_6;
  } external;

} board_pins_t;

class PanelLan : public lgfx::LGFX_Device {
  panelLan_board_t _board;

public:
  PanelLan(panelLan_board_t board);
  board_pins_t pins;
  bool init_impl(bool use_reset, bool use_clear);
};
