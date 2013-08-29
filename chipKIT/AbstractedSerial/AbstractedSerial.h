#ifndef _ABSTRACTED_SERIAL
#define _ABSTRACTED_SERIAL

#include <WProgram.h>

#ifdef _ABSTRACTED_SERIAL_SOFTWARE
#include <SoftwareSerial.h>
#endif

class AbstractedSerial : public Print {
    private:
        HardwareSerial *hws;
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
        USBSerial *uss;
#endif
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
        SoftwareSerial *sws;
#endif
        unsigned char connectedLine;

    public:
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
        AbstractedSerial(SoftwareSerial *s);
        AbstractedSerial(SoftwareSerial *s, unsigned char conn);
#endif
        AbstractedSerial(HardwareSerial *h, unsigned char conn);
        AbstractedSerial(HardwareSerial *h);
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
        AbstractedSerial(USBSerial *u);
#endif
        operator int();
        void write(unsigned char c);
        int read();
        void begin(unsigned long b);
        unsigned int available();
};
 
#endif
