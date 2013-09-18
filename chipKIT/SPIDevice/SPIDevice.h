#ifndef _SPIDEVICE_H
#define _SPIDEVICE_H

#define OPT_BOARD_INTERNAL

#if (ARDUINO >= 100) 
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

#include <p32_defs.h>

#define SPI_CON_FRMEN       (1<<31)
#define SPI_CON_FRMSYNC     (1<<30)
#define SPI_CON_FRMPOL      (1<<29)
#define SPI_CON_MSSEN       (1<<28)
#define SPI_CON_FRMSYPW     (1<<27)
#define SPI_CON_FRMCNT(X)   (((X) & 0b111) << 24)
#define SPI_CON_MCLKSEL     (1<<23)
#define SPI_CON_SPIFE       (1<<17)
#define SPI_CON_ENHBUF      (1<<16)
#define SPI_CON_ON          (1<<15)
#define SPI_CON_SIDL        (1<<13)
#define SPI_CON_DISSDO      (1<<12)
#define SPI_CON_MODE8       (0b00 << 10)
#define SPI_CON_MODE16      (0b01 << 10)
#define SPI_CON_MODE32      (0b10 << 10)
#define SPI_CON_SMP         (1<<9)
#define SPI_CON_CKE         (1<<8)
#define SPI_CON_SSEN        (1<<7)
#define SPI_CON_CKP         (1<<6)
#define SPI_CON_MSTEN       (1<<5)
#define SPI_CON_DISSDI      (1<<4)
#define SPI_CON_STXISEL(X)  (((X) & 0b11) << 2)
#define SPI_CON_SRXISEL(X)  (((X) & 0b11))

#define SPI_MODE0   SPI_CON_CKE
#define SPI_MODE1   0
#define SPI_MODE2   (SPI_CON_CKP | SPI_CON_CKE)
#define SPI_MODE3   SPI_CON_CKP

class SPIDevice {
    public:
        SPIDevice(uint8_t channel, uint8_t cs, uint8_t mode = 0, uint32_t speed = 10000000UL);
        void begin();
        uint32_t transfer(uint32_t data, boolean cont=false);
        uint32_t transfer8(uint32_t data, boolean cont=false);
        uint32_t transfer16(uint32_t data, boolean cont=false);
        uint32_t transfer32(uint32_t data, boolean cont=false);
        void blockread(uint8_t *data, uint32_t len);
        void blockread8(uint8_t *data, uint32_t len);
        void blockread16(uint16_t *data, uint32_t len);
        void blockread32(uint32_t *data, uint32_t len);
        void blockwrite(uint8_t *data, uint32_t len);
        void blockwrite8(uint8_t *data, uint32_t len);
        void blockwrite16(uint16_t *data, uint32_t len);
        void blockwrite32(uint32_t *data, uint32_t len);

    protected:
        uint8_t _channel;
        uint8_t _cs;
        uint8_t _mode;
        uint32_t _speed;

        volatile p32_spi *_spi;

        void initTransfer(uint8_t bits);

};

#endif
