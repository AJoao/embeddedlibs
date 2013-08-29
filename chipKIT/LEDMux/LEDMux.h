#ifndef _LEDMUX_H
#define _LEDMUX_H

#include <WProgram.h>
#include <Print.h>

#define USE_TIMER 5 
#define UPDATE_TIME 500

class LEDMux : public Print
{
    private:
        volatile unsigned char digit;

        volatile unsigned char nAnodes;
        volatile unsigned char nCathodes;

        volatile unsigned char anodes[8];
        volatile unsigned char cathodes[8];
        volatile unsigned char brightness[8];
        volatile unsigned char dp;
        unsigned char cpos;
        unsigned int delay;

    public:
        volatile char buffer[8];

    public:
        LEDMux(unsigned char a, unsigned char c);
        LEDMux(unsigned char a, unsigned char c, unsigned int d);
        void update();

        void setAnodes(unsigned char a0);
        void setAnodes(unsigned char a0, unsigned char a1);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5, unsigned char a6);
        void setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5, unsigned char a6, unsigned char a7);

        void setCathodes(unsigned char c0);
        void setCathodes(unsigned char c0, unsigned char c1);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5, unsigned char c6);
        void setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5, unsigned char c6, unsigned char c7);
        virtual void write(uint8_t c);
        virtual void write(char *s, int c);

        void setBrightness(unsigned char);
        void setBrightness(unsigned char, unsigned char);

        void setDecimalPoint(unsigned char);
        void setDecimalPoint(unsigned char, unsigned char);

        void begin();
};

#endif

