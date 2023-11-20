#include "boards.h"

using namespace lgfx::v1;

#ifdef CONFIG_IDF_TARGET_ESP32
Panel_Device* panel_load_from_sc01(board_pins_t* pins);
#elif CONFIG_IDF_TARGET_ESP32S3
Panel_Device* panel_load_from_sc01_plus(board_pins_t* pins);
Panel_Device* panel_load_from_sc02(board_pins_t* pins);
Panel_Device* panel_load_from_sc05(board_pins_t* pins);
Panel_Device* panel_load_from_bc02(board_pins_t* pins);
Panel_Device* panel_load_from_kc01(board_pins_t* pins);
#endif

PanelLan::PanelLan(panelLan_board_t board) {
  _board = board;
  setPanel(nullptr);
}

bool PanelLan::init_impl(bool use_reset, bool use_clear) {
  Panel_Device* panel = nullptr;
  switch (_board) {
#ifdef CONFIG_IDF_TARGET_ESP32
    case BOARD_SC01: panel = panel_load_from_sc01(&pins); break;
#elif CONFIG_IDF_TARGET_ESP32S3
    case BOARD_SC01_PLUS: panel = panel_load_from_sc01_plus(&pins); break;
    case BOARD_SC02: panel = panel_load_from_sc02(&pins); break;
    case BOARD_SC05: panel = panel_load_from_sc05(&pins); break;
    case BOARD_KC01: panel = panel_load_from_kc01(&pins); break;
    case BOARD_BC02: panel = panel_load_from_bc02(&pins); break;
#endif
    default: break;
  }

  if (panel == nullptr) {
    assert(0);
  }

  setPanel(panel);
  return LGFX_Device::init_impl(false, use_clear);
}
