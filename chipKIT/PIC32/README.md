PIC32
=====

This library is intended for use with the MCPHID bootloader used by the
Majenko Technologies boards.  It can be adapted (by removing the bootloader()
function) for use on other pic32 boards.

There is currently two functions:

    reset();

Reset the chip - just like pressing the reset button.

    bootloader();

Reset the chip and enter the bootloader.  Makes use of the bootloader key BLRKEY located
at 0x80000000
