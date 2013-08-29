#ifndef _THERMOCOUPLE_H
#define _THERMOCOUPLE_H

#include <DSPI.h>

struct tcData {
    union {
        unsigned int value;
        struct {
            unsigned oc:1;
            unsigned scg:1;
            unsigned scv:1;
            unsigned resb:1;
            unsigned internal:12;
            unsigned fault:1;
            unsigned resa:1;
            unsigned external:14;
        };
    };
}__attribute__((packed));

#define TC_FAULT 1
#define TC_OC 2
#define TC_SCG 4
#define TC_SCV 8

class Thermocouple {
    private:
        struct tcData data;

        float internal_;
        float external_;
        unsigned char flags;

        DSPI *spi;
        unsigned char ss_pin;

    public:
        Thermocouple(DSPI *spibus, unsigned char ss);
        void begin();
        void update();
        float internal();
        float external();
        unsigned char status();
};

#endif

