/*----------------------------------------------------------------------------/
  Lovyan GFX - Graphics library for embedded devices.

Original Source:
 https://github.com/lovyan03/LovyanGFX/

Licence:
 [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)

Author:
 [lovyan03](https://twitter.com/lovyan03)

Contributors:
 [ciniml](https://github.com/ciniml)
 [mongonta0716](https://github.com/mongonta0716)
 [tobozo](https://github.com/tobozo)
/----------------------------------------------------------------------------*/
#include "Touch_CST3240.hpp"

#include <LovyanGFX.hpp>
#include <Wire.h>  
// #include "../../internal/algorithm.h"
// #include "../platforms/common.hpp"

namespace lgfx
{
 inline namespace v1
 {
//---------------------------------------------------------------------------

  static bool _I2C_Read(uint8_t Driver_addr, uint16_t Reg_addr, uint8_t *Reg_data, uint32_t Length)
  {
    Wire.beginTransmission(Driver_addr);
    Wire.write((uint8_t)(Reg_addr >> 8)); 
    Wire.write((uint8_t)Reg_addr);      
    if ( Wire.endTransmission(true)){
      // printf("The I2C transmission fails. - I2C Read\r\n");
      return false;
    }
    Wire.requestFrom(Driver_addr, Length);
    for (int i = 0; i < Length; i++) {
      *Reg_data++ = Wire.read();
    }
    return true;
  }

  static bool _I2C_Write(uint8_t Driver_addr, uint16_t Reg_addr, const uint8_t *Reg_data, uint32_t Length)
  {
    Wire.beginTransmission(Driver_addr);
    Wire.write((uint8_t)(Reg_addr >> 8)); 
    Wire.write((uint8_t)Reg_addr);        
    for (int i = 0; i < Length; i++) {
      Wire.write(*Reg_data++);
    }
    if ( Wire.endTransmission(true))
    {
      // printf("The I2C transmission fails. - I2C Write\r\n");
      return false;
    }
    return true;
  }

  bool Touch_CST3240::init(void)
  {
    if (_inited) return true;

    if (isSPI()) return false;

    Wire.begin(_cfg.pin_sda, _cfg.pin_scl, _cfg.freq);

    if (_cfg.pin_rst >= 0)
    {
      lgfx::pinMode(_cfg.pin_rst, pin_mode_t::output);
      lgfx::gpio_lo(_cfg.pin_rst);
      lgfx::delay(1);
      lgfx::gpio_hi(_cfg.pin_rst);
      lgfx::delay(1);
    }

    if (_cfg.pin_int >= 0)
    {
      lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::input);
    }
    
    if( _I2C_Write(_cfg.i2c_addr, 0xD109, NULL, 0) == true){
      _inited = true;
      return true;
    }

    return false;
  }

  void Touch_CST3240::wakeup(void)
  {
    if (!_inited) return;
    if (_cfg.pin_int < 0) return;
    lgfx::gpio_hi(_cfg.pin_int);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::output);
    delay(5);
    lgfx::lgfxPinMode(_cfg.pin_int, pin_mode_t::input);
  }

  void Touch_CST3240::sleep(void)
  {
    if (!_inited) return;

  }
  
  
  uint_fast8_t Touch_CST3240::getTouchRaw(touch_point_t *__restrict tp, uint_fast8_t count)
  {
      if (!_inited) return 0;
      if (count > 5) { count = 5; }

      uint8_t data[7] = {0x0};
    
      _I2C_Read(_cfg.i2c_addr, 0xD000, data, 7);

      uint8_t event = (data[0] & 0x04) ? 1 : 0;
      if (event == 0) {
        return 0;
      }

      uint8_t touchPoints = data[0] & 0x0F; 
      if (count > touchPoints) { count = touchPoints; }
      for (size_t idx = 0; idx < count; ++idx)
      {
          tp[idx].x = (data[1] << 4) | ((data[3] >> 4) & 0x0F);
          tp[idx].y = (data[2] << 4) | (data[3] & 0x0F);
          tp[idx].size = 1;
          tp[idx].id = idx;
          // printf("%d %d \n",tp[idx].x, tp[idx].y);
      }

      uint8_t Over = 0xAB;
      _I2C_Write(_cfg.i2c_addr, 0xD000, &Over, 1);

      return count;
  }

//----------------------------------------------------------------------------
 }
}
