#include <WProgram.h>
#include <LEDMux.h>
#include <p32xxxx.h>
#include <sys/attribs.h>

#include <stdarg.h>

LEDMux *internalLEDMuxPointer;

// List of characters starting from space
const unsigned char digits[] = {
    0b00000000,     // Space
    0b10000110,     // !
    0b00100010,     // "
    0b01110110,     // #
    0b01101101,     // $
    0b01010010,     // %
    0b01111111,     // &
    0b00100000,     // '
    0b00111001,     // (
    0b00001111,     // )
    0b01110110,     // *
    0b01000110,     // +
    0b00001100,     // ,
    0b01000000,     // -
    0b10000000,     // .
    0b01010010,     // /
	0b00111111,     // 0
	0b00000110,     // 1
	0b01011011,     // 2
	0b01001111,     // 3
	0b01100110,     // 4
	0b01101101,     // 5
	0b01111101,     // 6
	0b00000111,     // 7
	0b01111111,     // 8
	0b01101111,     // 9
    0b00001001,     // :
    0b00001101,     // ;
    0b01011000,     // <
    0b01001000,     // =
    0b01001100,     // >
    0b01010011,     // ?
    0b01011111,     // @
    0b01110111,     // A
    0b01111100,     // B
    0b00111001,     // C
    0b01011110,     // D
    0b01111001,     // E
    0b01110001,     // F
    0b00111101,     // G
    0b01110110,     // H
    0b00000110,     // I
    0b00011110,     // J
    0b01110101,     // K
    0b00111000,     // L
    0b00110111,     // M
    0b00110111,     // N
    0b00111111,     // O
    0b01110011,     // P
    0b01100111,     // Q
    0b01010000,     // R
    0b01101101,     // S
    0b01111000,     // T
    0b00111110,     // U
    0b00111110,     // V
    0b00111110,     // W
    0b01110110,     // X
    0b01101110,     // Y
    0b01011011,     // Z
    0b00111001,     // [
    0b01100100,     // backslash
    0b00001111,     // ]
    0b00100011,     // ^
    0b00001000,     // _
    0b00000011,     // `
    0b01110111,     // A
    0b01111100,     // B
    0b00111001,     // C
    0b01011110,     // D
    0b01111001,     // E
    0b01110001,     // F
    0b00111101,     // G
    0b01110110,     // H
    0b00000110,     // I
    0b00011110,     // J
    0b01110101,     // K
    0b00111000,     // L
    0b00110111,     // M
    0b00110111,     // N
    0b00111111,     // O
    0b01110011,     // P
    0b01100111,     // Q
    0b01010000,     // R
    0b01101101,     // S
    0b01111000,     // T
    0b00111110,     // U
    0b00111110,     // V
    0b00111110,     // W
    0b01110110,     // X
    0b01101110,     // Y
    0b01011011,     // Z
    0b00111001,     // {
    0b00110000,     // |
    0b00001111,     // }
    0b01000000,     // ~
    0b01100011,     // DEL
    0b00010000,    
    0b00110000,   
    0b00000100,    
    0b00010100,    
    0b00110100,    
    0b00000110,    
    0b00010110,    
    0b00110110,    
};

LEDMux::LEDMux (unsigned char a, unsigned char c)
{
    unsigned char i;
	this->nAnodes = a;
	this->nCathodes = c;
    this->cpos = 0;

    for (i=0; i<8; i++) {
        this->buffer[i] = ' ';
    }

    this->delay = UPDATE_TIME;
}

LEDMux::LEDMux (unsigned char a, unsigned char c, unsigned int d)
{
    unsigned char i;
	this->nAnodes = a;
	this->nCathodes = c;
    this->cpos = 0;

    for (i=0; i<8; i++) {
        this->buffer[i] = ' ';
    }

    this->delay = d;
}

void LEDMux::begin()
{
#if USE_TIMER == 2
	T2CONbits.TCKPS = 0b010; // 1:4
	PR2 = this->delay;
	IPC2bits.T2IP = 6;
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;
	T2CONbits.TON = 1;
#endif
#if USE_TIMER == 3
	T3CONbits.TCKPS = 0b010; // 1:4
	PR3 = this->delay;
	IPC3bits.T3IP = 6;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
	T3CONbits.TON = 1;
#endif
#if USE_TIMER == 4
	T4CONbits.TCKPS = 0b010; // 1:4
	PR4 = this->delay;
	IPC4bits.T4IP = 6;
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;
	T4CONbits.TON = 1;
#endif
#if USE_TIMER == 5
	T5CONbits.TCKPS = 0b010; // 1:4
	PR5 = this->delay;
	IPC5bits.T5IP = 6;
	IFS0bits.T5IF = 0;
	IEC0bits.T5IE = 1;
	T5CONbits.TON = 1;
#endif

	internalLEDMuxPointer = this;
}

void LEDMux::setAnodes(unsigned char a0)
{
    this->anodes[0] = a0;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;
    this->anodes[3] = a3;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);

    pinMode(this->anodes[3], OUTPUT);
    digitalWrite(this->anodes[3], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;
    this->anodes[3] = a3;
    this->anodes[4] = a4;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);

    pinMode(this->anodes[3], OUTPUT);
    digitalWrite(this->anodes[3], LOW);

    pinMode(this->anodes[4], OUTPUT);
    digitalWrite(this->anodes[4], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;
    this->anodes[3] = a3;
    this->anodes[4] = a4;
    this->anodes[5] = a5;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);

    pinMode(this->anodes[3], OUTPUT);
    digitalWrite(this->anodes[3], LOW);

    pinMode(this->anodes[4], OUTPUT);
    digitalWrite(this->anodes[4], LOW);

    pinMode(this->anodes[5], OUTPUT);
    digitalWrite(this->anodes[5], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5, unsigned char a6)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;
    this->anodes[3] = a3;
    this->anodes[4] = a4;
    this->anodes[5] = a5;
    this->anodes[6] = a6;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);

    pinMode(this->anodes[3], OUTPUT);
    digitalWrite(this->anodes[3], LOW);

    pinMode(this->anodes[4], OUTPUT);
    digitalWrite(this->anodes[4], LOW);

    pinMode(this->anodes[5], OUTPUT);
    digitalWrite(this->anodes[5], LOW);

    pinMode(this->anodes[6], OUTPUT);
    digitalWrite(this->anodes[6], LOW);
}

void LEDMux::setAnodes(unsigned char a0, unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4, unsigned char a5, unsigned char a6, unsigned char a7)
{
    this->anodes[0] = a0;
    this->anodes[1] = a1;
    this->anodes[2] = a2;
    this->anodes[3] = a3;
    this->anodes[4] = a4;
    this->anodes[5] = a5;
    this->anodes[6] = a6;
    this->anodes[7] = a7;

    pinMode(this->anodes[0], OUTPUT);
    digitalWrite(this->anodes[0], LOW);

    pinMode(this->anodes[1], OUTPUT);
    digitalWrite(this->anodes[1], LOW);

    pinMode(this->anodes[2], OUTPUT);
    digitalWrite(this->anodes[2], LOW);

    pinMode(this->anodes[3], OUTPUT);
    digitalWrite(this->anodes[3], LOW);

    pinMode(this->anodes[4], OUTPUT);
    digitalWrite(this->anodes[4], LOW);

    pinMode(this->anodes[5], OUTPUT);
    digitalWrite(this->anodes[5], LOW);

    pinMode(this->anodes[6], OUTPUT);
    digitalWrite(this->anodes[6], LOW);

    pinMode(this->anodes[7], OUTPUT);
    digitalWrite(this->anodes[7], LOW);
}

void LEDMux::setCathodes(unsigned char c0)
{
    this->cathodes[0] = c0;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;
    this->cathodes[3] = c3;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);

    pinMode(c3, OUTPUT);
    digitalWrite(c3, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;
    this->cathodes[3] = c3;
    this->cathodes[4] = c4;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);

    pinMode(c3, OUTPUT);
    digitalWrite(c3, LOW);

    pinMode(c4, OUTPUT);
    digitalWrite(c4, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;
    this->cathodes[3] = c3;
    this->cathodes[4] = c4;
    this->cathodes[5] = c5;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);

    pinMode(c3, OUTPUT);
    digitalWrite(c3, LOW);

    pinMode(c4, OUTPUT);
    digitalWrite(c4, LOW);

    pinMode(c5, OUTPUT);
    digitalWrite(c5, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5, unsigned char c6)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;
    this->cathodes[3] = c3;
    this->cathodes[4] = c4;
    this->cathodes[5] = c5;
    this->cathodes[6] = c6;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);

    pinMode(c3, OUTPUT);
    digitalWrite(c3, LOW);

    pinMode(c4, OUTPUT);
    digitalWrite(c4, LOW);

    pinMode(c5, OUTPUT);
    digitalWrite(c5, LOW);

    pinMode(c6, OUTPUT);
    digitalWrite(c6, LOW);
}

void LEDMux::setCathodes(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4, unsigned char c5, unsigned char c6, unsigned char c7)
{
    this->cathodes[0] = c0;
    this->cathodes[1] = c1;
    this->cathodes[2] = c2;
    this->cathodes[3] = c3;
    this->cathodes[4] = c4;
    this->cathodes[5] = c5;
    this->cathodes[6] = c6;
    this->cathodes[7] = c7;

    pinMode(c0, OUTPUT);
    digitalWrite(c0, LOW);

    pinMode(c1, OUTPUT);
    digitalWrite(c1, LOW);

    pinMode(c2, OUTPUT);
    digitalWrite(c2, LOW);

    pinMode(c3, OUTPUT);
    digitalWrite(c3, LOW);

    pinMode(c4, OUTPUT);
    digitalWrite(c4, LOW);

    pinMode(c5, OUTPUT);
    digitalWrite(c5, LOW);

    pinMode(c6, OUTPUT);
    digitalWrite(c6, LOW);

    pinMode(c7, OUTPUT);
    digitalWrite(c7, LOW);
}

void LEDMux::update()
{
    unsigned char v;
    unsigned char i;
    unsigned char c;
    static unsigned char b = 0;

    for (i=0; i<this->nAnodes; i++) {
		digitalWrite(this->anodes[i], 0);
	}
    for (i=0; i<this->nCathodes; i++) {
		digitalWrite(this->cathodes[i], 0);
	}

    this->digit++;
    if (this->digit >= this->nCathodes) {
			this->digit = 0;
            b++;
            b = b & 0b11111;
    }

    if (this->brightness[this->digit] == 0) {
        return;
    }

    if (b < (1<<(this->brightness[this->digit]-1))) {
        c = buffer[this->digit];
        if (c < ' ') {
            c = ' ';
        }
    
        if (c > 135) {
            c = ' ';
        }
        c = c - ' ';

        v = digits[c];
        if ((this->dp >> this->digit) & 1) {
            v = v | 0b10000000;
        }

        for (i=0; i<this->nAnodes; i++) {
            digitalWrite(this->anodes[i], v & (1<<i) ? 1 : 0);
        }
        digitalWrite(this->cathodes[this->digit], HIGH);
    }
}

void LEDMux::write(uint8_t c)
{
    int i;
    switch(c) {
        case '\r':
            this->cpos = 0;
            break;
        case '\n':
            for (i=0; i<this->nCathodes; i++) {
                this->buffer[i] = ' ';
            }
            this->cpos = 0;
            break;
        default:
            this->buffer[this->cpos] = c;
            this->cpos++;
            if (this->cpos >= this->nCathodes) {
                cpos = 0;
            }
            break;
    }
}

void LEDMux::write(char *s, int c)
{
    int i;
    for (i=0; i<c; i++) {
        this->write(s[i]);
    }
}

void LEDMux::setBrightness(unsigned char b)
{
    unsigned char i;
    for (i=0; i<this->nCathodes; i++) {
        this->setBrightness(i, b);
    }
}

void LEDMux::setBrightness(unsigned char c, unsigned char b)
{
    this->brightness[c] = b;
}

void LEDMux::setDecimalPoint(unsigned char d)
{
    this->dp = d;
}

void LEDMux::setDecimalPoint(unsigned char pos, unsigned char val)
{
    this->dp &= ~(1<<pos);
    if (val) {
        this->dp |= (1<<pos);
    }
}

extern "C" {
#if USE_TIMER == 2
    void __ISR(_TIMER_2_VECTOR, IPL6) _T2Interrupt(void)
#endif
#if USE_TIMER == 3
    void __ISR(_TIMER_3_VECTOR, IPL6) _T3Interrupt(void)
#endif
#if USE_TIMER == 4
    void __ISR(_TIMER_4_VECTOR, IPL6) _T4Interrupt(void)
#endif
#if USE_TIMER == 5
    void __ISR(_TIMER_5_VECTOR, IPL6) _T5Interrupt(void)
#endif
    {
        if (internalLEDMuxPointer) {
            internalLEDMuxPointer->update();
        }
#if USE_TIMER == 2
        IFS0bits.T2IF = 0;
#endif
#if USE_TIMER == 3
        IFS0bits.T3IF = 0;
#endif
#if USE_TIMER == 4
        IFS0bits.T4IF = 0;
#endif
#if USE_TIMER == 5
        IFS0bits.T5IF = 0;
#endif
    }
}

