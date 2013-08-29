#ifndef _PIC32_H
#define _PIC32_H

#define BLRKEY      *(volatile unsigned*)(0x80000000)

extern void reset();
extern void bootloader();

#endif
