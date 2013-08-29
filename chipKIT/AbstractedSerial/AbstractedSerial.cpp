#include <AbstractedSerial.h>

#ifdef _ABSTRACTED_SERIAL_SOFTWARE
AbstractedSerial::AbstractedSerial(SoftwareSerial *s, unsigned char conn) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    uss = NULL;
#endif
    hws = NULL;
    sws = s;
    connectedLine = conn;
}
AbstractedSerial::AbstractedSerial(SoftwareSerial *s) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    uss = NULL;
#endif
    hws = NULL;
    sws = NULL;
    connectedLine = 255;
}
#endif

AbstractedSerial::AbstractedSerial(HardwareSerial *h, unsigned char conn) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    uss = NULL;
#endif
    hws = h;
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    sws = NULL;
#endif
    connectedLine = conn;
}
AbstractedSerial::AbstractedSerial(HardwareSerial *h) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    uss = NULL;
#endif
    hws = h;
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    sws = NULL;
#endif
    connectedLine = 255;
}

#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
AbstractedSerial::AbstractedSerial(USBSerial *u) {
    uss = u;
    hws = NULL;
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    sws = NULL;
#endif
    connectedLine = 255;
}
#endif

AbstractedSerial::operator int() {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    if (uss) {
        return *uss;
    }
#endif
    if (connectedLine != 255) {
        return digitalRead(connectedLine);
    }
    return 1;
}
void AbstractedSerial::write(unsigned char c) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    if (uss) {
        uss->write(c);
    }
#endif
    if (hws) {
        hws->write(c);
    }
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    if (sws) {
        sws->write(c);
    }
#endif
}

int AbstractedSerial::read() {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    if (uss) {
        return uss->read();
    }
#endif
    if (hws) {
        return hws->read();
    }
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    if (sws) {
        return sws->read();
    }
#endif
    return -1;
}
void AbstractedSerial::begin(unsigned long b) {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    if (uss) {
        uss->begin(b);
    }
#endif
    if (hws) {
        hws->begin(b);
    }
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    if (sws) {
        sws->begin(b);
    }
#endif
}
unsigned int AbstractedSerial::available() {
#if defined(_USB) && defined(_USE_USB_FOR_SERIAL_)
    if (uss) {
        return uss->available();
    }
#endif
    if (hws) {
        return hws->available();
    }
#ifdef _ABSTRACTED_SERIAL_SOFTWARE
    if (sws) {
        return sws->available();
    }
#endif
    return 0;
}
