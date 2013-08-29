AbstractedSerial
================

A wrapper class for containing different kinds of Serial
object to provide a single standard class interface to them
all.

#include <AbstractSerial.h> for USB and Hardware serial.
#include <AbstractSerial_software.h> for USB, Hardware and Software serial.  Also
requires #include <SoftwareSerial.h>

Create an AbstractedSerial copy of a normal serial port:

    AbstractedSerial mySerial(&Serial);

Can also include an "activation" line to monitor for connection/disconnaction - useful for
bluetooth modules that provide a paired signal

    AbstractedSerial mySerial(&Serial2, 3);

All normal Serial methods are available:

    mySerial.begin(9600);

    while(mySerial.availabl() > 0) {
        mySerial.print(mySerial.read(), HEX);
    }

Also supports USB style connection reporting.  For USB it returns the USB connected state.  For others,
if a monitoring line is provided, it returns the state of that line.  Otherwise it assumes it's connected:

    if (mySerial) {
        delay(100);
        mySerial.println("Hello there");
    }

