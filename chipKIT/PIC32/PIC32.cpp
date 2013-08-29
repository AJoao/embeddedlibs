#include <PIC32.h>

#include <p32xxxx.h>

void reset()
{
    SYSKEY = 0;
    SYSKEY = 0xaa996655;
    SYSKEY = 0x556699aa;

    /* Reset microcontroller */
    RSWRSTSET = 1;
    (void) RSWRST;
}

void bootloader()
{
    BLRKEY=0x12345678;
    reset();
}

