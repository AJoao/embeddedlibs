#include <SPIDevice.h>

SPIDevice::SPIDevice(uint8_t channel, uint8_t cs, uint8_t mode, uint32_t speed) : _channel(channel), _cs(cs), _mode(mode), _speed(speed) {

    _spi = NULL;

#ifdef _DSPI0_BASE
    if (_channel == 0) {
        _spi = (volatile p32_spi *)_DSPI0_BASE;
    }
#undef SPI_CHAN_OK
#define SPI_CHAN_OK
#endif

#ifdef _DSPI1_BASE 
    if (_channel == 1) {
        _spi = (volatile p32_spi *)_DSPI1_BASE;
    }
#undef SPI_CHAN_OK
#define SPI_CHAN_OK
#endif

#ifdef _DSPI2_BASE 
    if (_channel == 2) {
        _spi = (volatile p32_spi *)_DSPI2_BASE;
    }
#undef SPI_CHAN_OK
#define SPI_CHAN_OK
#endif

#ifdef _DSPI3_BASE 
    if (_channel == 3) {
        _spi = (volatile p32_spi *)_DSPI3_BASE;
    }
#undef SPI_CHAN_OK
#define SPI_CHAN_OK
#endif

#ifndef SPI_CHAN_OK
#error No SPI channels found for this board
#endif

}

void SPIDevice::begin() {
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    if (!_spi) {
        while (1) {
            digitalWrite(13, HIGH);
            delay(100);
            digitalWrite(13, LOW);
            delay(900);
        }
    }
    initTransfer(8);
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
}

void SPIDevice::initTransfer(uint8_t bits) {

    _spi->sxCon.clr = SPI_CON_ON;
    _spi->sxBrg.reg = (F_CPU / _speed + 1) / 2 - 1;
    _spi->sxCon.reg = SPI_CON_MSTEN;

    switch (_mode) {
        case 0:
            _spi->sxCon.set = SPI_MODE0;
            break;
        case 1:
            _spi->sxCon.set = SPI_MODE1;
            break;
        case 2:
            _spi->sxCon.set = SPI_MODE2;
            break;
        case 3:
            _spi->sxCon.set = SPI_MODE3;
            break;

    }
    switch (bits) {
        case 8:
            _spi->sxCon.set = SPI_CON_MODE8;
            break;
        case 16:
            _spi->sxCon.set = SPI_CON_MODE16;
            break;
        case 32:
            _spi->sxCon.set = SPI_CON_MODE32;
            break;
    }
    _spi->sxCon.set = SPI_CON_ON;
}

uint32_t SPIDevice::transfer(uint32_t data, boolean cont) {
    return transfer8(data, cont);
}

uint32_t SPIDevice::transfer8(uint32_t data, boolean cont) {
    uint32_t out;
    initTransfer(8);

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) {
        continue;
    }

    digitalWrite(_cs, LOW);
    _spi->sxBuf.reg = data;

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
        continue;
    }

    if (!cont) {
        digitalWrite(_cs, HIGH);
    }

    out = _spi->sxBuf.reg;

    return out;
}

uint32_t SPIDevice::transfer16(uint32_t data, boolean cont) {
    uint32_t out;
    initTransfer(16);

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) {
        continue;
    }

    digitalWrite(_cs, LOW);
    _spi->sxBuf.reg = data;

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
        continue;
    }

    if (!cont) {
        digitalWrite(_cs, HIGH);
    }

    out = _spi->sxBuf.reg;

    return out;
}

uint32_t SPIDevice::transfer32(uint32_t data, boolean cont) {
    uint32_t out;
    initTransfer(32);

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPITBE)) == 0) {
        continue;
    }

    digitalWrite(_cs, LOW);
    _spi->sxBuf.reg = data;

    while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
        continue;
    }

    if (!cont) {
        digitalWrite(_cs, HIGH);
    }

    out = _spi->sxBuf.reg;

    return out;
}

void SPIDevice::blockread(uint8_t *data, uint32_t len) {
    blockread8(data, len);
}

void SPIDevice::blockread8(uint8_t *data, uint32_t len) {
    initTransfer(8);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = 0xFF;
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        data[i] = _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}

void SPIDevice::blockread16(uint16_t *data, uint32_t len) {
    initTransfer(16);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = 0xFF;
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        data[i] = _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}

void SPIDevice::blockread32(uint32_t *data, uint32_t len) {
    initTransfer(32);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = 0xFF;
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        data[i] = _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}

void SPIDevice::blockwrite(uint8_t *data, uint32_t len) {
    blockwrite8(data, len);
}

void SPIDevice::blockwrite8(uint8_t *data, uint32_t len) {
    initTransfer(8);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = data[i];
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        (void) _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}

void SPIDevice::blockwrite16(uint16_t *data, uint32_t len) {
    initTransfer(16);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = data[i];
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        (void) _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}

void SPIDevice::blockwrite32(uint32_t *data, uint32_t len) {
    initTransfer(32);
    digitalWrite(_cs, LOW);
    for (uint32_t i = 0; i < len; i++) {
        _spi->sxBuf.reg = data[i];
        while ((_spi->sxStat.reg & (1 << _SPISTAT_SPIRBF)) == 0) {
            continue;
        }
        (void) _spi->sxBuf.reg;
    }
    digitalWrite(_cs, HIGH);
}
