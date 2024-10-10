# aw9523 扩展IO使用

## 环境
安装：Adafruit_AW9523以及它所需要的Adafruit_BusIO

## 例子：BOARD_SC05初始化
1. I2C配置：屏幕触摸与aw9523使用I2C相同，触摸配置处使用`I2C_NUM_1`，频率为400000
    - `cfg.i2c_port = I2C_NUM_1;`
    - `cfg.freq = 400000;`

2. 初始化I2C
    - `Wire1.begin(I2C_SDA_PIN, I2C_SCL_PIN);`
    - `Wire1.setClock(400000);`
    - `aw.begin(AW9523_I2C_ADDR, &Wire1)`

## 问题：触摸失灵

- **BOARD_SC05**：aw9523_P10脚连接屏幕复位脚,aw9523_P11脚连接触摸复位脚
- `begin`函数初始化过程中会影响触摸复位脚:
```cpp
bool Adafruit_AW9523::begin(uint8_t addr, TwoWire *wire) {
    ...
    configureDirection(0x0); // all inputs!
    ...
}
```

### 问题解决
```cpp
aw.pinMode(LCD_RST_Pin, OUTPUT);
aw.pinMode(TP_RST_Pin, OUTPUT);
aw.digitalWrite(LCD_RST_Pin, HIGH);
aw.digitalWrite(TP_RST_Pin, HIGH);
```


