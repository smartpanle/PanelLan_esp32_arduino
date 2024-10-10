# AW9523 Extended IO Usage

## Environment
Installation: `Adafruit_AW9523` and its required `Adafruit_BusIO`.

## Example: BOARD_SC05 Initialization
1. I2C Configuration: The screen touch and AW9523 share the same I2C bus. The touch is configured to use `I2C_NUM_1` with a frequency of 400000.
   - `cfg.i2c_port = I2C_NUM_1;`
   - `cfg.freq = 400000;`

2. I2C Initialization
```cpp
   Wire1.begin(I2C_SDA_PIN, I2C_SCL_PIN);
   Wire1.setClock(400000);
   aw.begin(AW9523_I2C_ADDR, &Wire1);
``` 

## Issue: Touch Failure

- **BOARD_SC05**: The AW9523's P10 pin is connected to the screen reset pin, and the P11 pin is connected to the touch reset pin.
- During the `begin` function, the initialization affects the touch reset pin:
```cpp
bool Adafruit_AW9523::begin(uint8_t addr, TwoWire *wire) {
    ...
    configureDirection(0x0); // all inputs!
    ...
}
```

## Solution
```cpp
aw.pinMode(LCD_RST_Pin, OUTPUT);
aw.pinMode(TP_RST_Pin, OUTPUT);
aw.digitalWrite(LCD_RST_Pin, HIGH);
aw.digitalWrite(TP_RST_Pin, HIGH);
```