#include "AW9523B.h"

AW9523B::AW9523B() {
#ifdef AW9523B_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
    DEBUG_PRINTLN("Call Constructor");
}

AW9523B::AW9523B(int8_t sda, int8_t scl, int8_t ad0, int8_t ad1) 
    : sda(sda), scl(scl), ad0(ad0), ad1(ad1) {
#ifdef AW9523B_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
    DEBUG_PRINTLN("Call Constructor");
}

AW9523B::~AW9523B() {}

void AW9523B::begin() {
    delay(100); // Initial wait after Power Up
    lgfx::i2c::init(AW9523B_I2C_PORT, sda, scl); // Initialize I2C
}

void AW9523B::reset() {
#ifdef AW9523B_DEBUG
    DEBUG_PRINTLN("Resetting...");
#endif
    writeByte(AW9523B_ADDR_RESET, 0);
#ifdef AW9523B_DEBUG
    DEBUG_PRINTLN("Reset Finished");
#endif
}

void AW9523B::setAddress(uint8_t _ad0, uint8_t _ad1) {
    ad0 = _ad0;
    ad1 = _ad1;
}

uint8_t AW9523B::readPort(uint8_t port) {
    DEBUG_PRINT("Read Port %d\n", port);
    if (port == 0) {
        return readByte(AW9523B_ADDR_INPUT0);
    } else if (port == 1) {
        return readByte(AW9523B_ADDR_INPUT1);
    } else {
        DEBUG_PRINT("Read Port is Invalid");
        return 0;
    }
}

void AW9523B::writePort(uint8_t port, uint8_t data) {
    DEBUG_PRINT("Write Port %d / Data 0x%02x\n", port, data);
    if (port == 0) {
        writeByte(AW9523B_ADDR_OUTPUT0, data);
    } else if (port == 1) {
        writeByte(AW9523B_ADDR_OUTPUT1, data);
    } else {
        DEBUG_PRINT("Write Port is Invalid");
    }
}

void AW9523B::setConfig(uint8_t port, uint8_t mode) {
    if (port == 0) {
        writeByte(AW9523B_ADDR_CONFIG0, mode);
    } else if (port == 1) {
        writeByte(AW9523B_ADDR_CONFIG1, mode);
    }
}

void AW9523B::setInterrupt(uint8_t port, uint8_t flag) {
    if (port == 0) {
        writeByte(AW9523B_ADDR_INT0, flag);
    } else if (port == 1) {
        writeByte(AW9523B_ADDR_INT1, flag);
    }
}

uint8_t AW9523B::readID() {
    return readByte(AW9523B_ADDR_ID);
}

void AW9523B::setGlobalControl(GPIO_DRV_MODE_Enum mode, uint8_t range) {
    uint8_t data = ((mode & 0x01) << 4) | (range & 0x03);
    writeByte(AW9523B_ADDR_GCR, data);
}

void AW9523B::setLedMode(uint8_t port, uint8_t flag) {
    if (port == 0) {
        writeByte(AW9523B_ADDR_LEDMODE0, ~flag);
    } else if (port == 1) {
        writeByte(AW9523B_ADDR_LEDMODE1, ~flag);
    }
}

void AW9523B::setGpioMode(uint8_t port, uint8_t flag) {
    if (port == 0) {
        writeByte(AW9523B_ADDR_LEDMODE0, flag);
    } else if (port == 1) {
        writeByte(AW9523B_ADDR_LEDMODE1, flag);
    }
}

void AW9523B::setDimmer(uint8_t port, uint8_t subport, uint8_t dim) {
    if (port == 0) {
        writeByte(AW9523B_ADDR_DIM_BASE + 4 + subport, dim);
    } else if (port == 1) {
        if (subport < 4) {
            writeByte(AW9523B_ADDR_DIM_BASE + subport, dim);
        } else {
            writeByte(AW9523B_ADDR_DIM_BASE + 8 + subport, dim);
        }
    }
}

uint8_t AW9523B::readByte(uint8_t addr) {
    uint8_t rdData;
    cpp::result<void, lgfx::v1::error_t> result = lgfx::i2c::transactionWriteRead(AW9523B_I2C_PORT, AW9523B_I2C_BASE_ADDRESS + (ad1 << 1) + ad0, &addr, 1, &rdData, 1, AW9523B_I2C_FREQ); 
    if (result.has_value()) {
        return rdData;
    } else {
        DEBUG_PRINT("Read Error: %d\n", result.error());
        return 0;
    }
}

void AW9523B::writeByte(uint8_t addr, uint8_t data) {
    uint8_t buffer[2] = { addr, data };
    cpp::result<void, lgfx::v1::error_t> result = lgfx::i2c::transactionWrite(AW9523B_I2C_PORT, AW9523B_I2C_BASE_ADDRESS + (ad1 << 1) + ad0, buffer, sizeof(buffer), AW9523B_I2C_FREQ);
    if (!result.has_value()) {
        DEBUG_PRINT("Write Error: %d\n", result.error());
    }
}
