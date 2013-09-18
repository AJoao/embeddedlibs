#include <WProgram.h>
#include <PWMAudio.h>
#include <p32xxxx.h>
#include <sys/attribs.h>
#include <pins_arduino.h>

#include <stdarg.h>

PWMAudio *internalPWMAudioPointer;

PWMAudio::PWMAudio (unsigned char pin, unsigned int sfreq)
{
    this->audioPin = pin;
    this->sampleRate = sfreq;
    this->amplifierPin = 255;
    this->ampInvert = 0;
}
    
PWMAudio::PWMAudio (unsigned char pin, unsigned int sfreq, unsigned char amp)
{
    this->audioPin = pin;
    this->sampleRate = sfreq;
    this->amplifierPin = amp;
    this->ampInvert = 0;
}

void PWMAudio::invertAmpTrigger() {
    ampInvert = true;
}

void PWMAudio::ampOn()
{
    if (this->amplifierPin != 255) {
        digitalWrite(this->amplifierPin, ampInvert ? LOW : HIGH);
    }
}

void PWMAudio::ampOff()
{
    if (this->amplifierPin != 255) {
        digitalWrite(this->amplifierPin, ampInvert ? HIGH : LOW);
    }
}

void PWMAudio::begin()
{
	T4CONbits.TCKPS = 0b111; // 1:256
	PR4 = (F_CPU / 256) / this->sampleRate;

	IPC4bits.T4IP = 5;
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;
	T4CONbits.TON = 1;

    T3CONbits.TCKPS = 0b000; // 1:1
	T3CONbits.TON = 1;
    PR3 = AUDIO_BITS;
    
    switch (digital_pin_to_timer_PGM[this->audioPin]) {
        case _TIMER_OC1:
            OC1CONbits.OCTSEL = 1;
            OC1CONbits.OCM = 0b110;
            OC1CONbits.ON = 1;
            OC1RS = AUDIO_BITS >> 1;
            break;
        case _TIMER_OC2:
            OC2CONbits.OCTSEL = 1;
            OC2CONbits.OCM = 0b110;
            OC2CONbits.ON = 1;
            OC2RS = AUDIO_BITS >> 1;
            break;
        case _TIMER_OC3:
            OC3CONbits.OCTSEL = 1;
            OC3CONbits.OCM = 0b110;
            OC3CONbits.ON = 1;
            OC3RS = AUDIO_BITS >> 1;
            break;
        case _TIMER_OC4:
            OC4CONbits.OCTSEL = 1;
            OC4CONbits.OCM = 0b110;
            OC4CONbits.ON = 1;
            OC4RS = AUDIO_BITS >> 1;
            break;
        case _TIMER_OC5:
            OC5CONbits.OCTSEL = 1;
            OC5CONbits.OCM = 0b110;
            OC5CONbits.ON = 1;
            OC5RS = AUDIO_BITS >> 1;
            break;
    }
    if (this->amplifierPin != 255) {
        pinMode(this->amplifierPin, OUTPUT);
        this->ampOff();
    }

	internalPWMAudioPointer = this;
}

void PWMAudio::generateAudio()
{
    register int val = 0;
    register int temp;
    register int i;
    register struct sample *s;
    unsigned char channelsPlaying = 0;
    static unsigned long shutOff = 0;

    for (i=0; i<MAX_SAMPLES; i++) {
        s = &(this->samples[i]);
        if (s->sample != NULL) {
            if (s->delay > 0) {
                s->delay--;
            } else {
                channelsPlaying++;
                temp = s->sample[s->pos];
                if (s->volume < 255) {
                    temp = temp * s->volume;
                    temp = temp / 255;
                }
                val = mix(val, temp);
                s->pos++;

                s->spos += s->speed;
                if (s->spos > 100) {
                    s->pos++;
                    s->spos = 0;
                }
                if (s->spos < -100) {
                    s->pos--;
                    s->spos = 0;
                }

                if (s->pos >= s->len) {
                    s->sample = NULL;
                }
            }
        }
    }
    if (channelsPlaying > 0) {
        if (val < -32768) {
            val = -32768;
        }
        if (val > 32767) {
            val = 32767;
        }

        val = val + 32768;
        val = val >> (16 - AUDIO_LOG2);
        switch (digital_pin_to_timer_PGM[this->audioPin]) {
            case _TIMER_OC1:
                OC1RS = val;
                break;
            case _TIMER_OC2:
                OC2RS = val;
                break;
            case _TIMER_OC3:
                OC3RS = val;
                break;
            case _TIMER_OC4:
                OC4RS = val;
                break;
            case _TIMER_OC5:
                OC5RS = val;
                break;
        }

        this->ampOn();
        shutOff = 10;
    }

    if (shutOff > 0) {
        shutOff --;
        if (shutOff == 0) {
            this->ampOff();
        }
    }


}

void PWMAudio::queueSample(const short *sample, int length, int delay, int speed)
{
    this->queueSample(sample, length, delay, speed, 255);
}

void PWMAudio::queueSample(const short *sample, int length, int delay, int speed, int volume)
{
   int i;
    for (i=0; i<MAX_SAMPLES; i++) {
        if (this->samples[i].sample == NULL) {
            this->samples[i].sample = (short *)sample;
            this->samples[i].len = length;
            this->samples[i].delay = delay;
            this->samples[i].pos = 0;
            this->samples[i].speed = speed;
            this->samples[i].spos = 0;
            this->samples[i].volume = volume;
            return;
        }
    }
}

void PWMAudio::queueSingleSample(const short *sample, int length, int delay, int speed)
{
    this->queueSingleSample(sample, length, delay, speed, 255);
}

void PWMAudio::queueSingleSample(const short *sample, int length, int delay, int speed, int volume)
{
    int i;
    int found = 0;
    for (i=0; i<MAX_SAMPLES; i++) {
        if (this->samples[i].sample == sample) {
            found = 1;
        }
    }
    if (found == 0) {
        this->queueSample(sample, length, delay, speed, volume);
    }
}

int PWMAudio::mix(int a, int b)
{
    int z;
    unsigned int fa, fb, fz;
    fa = a + 32768;
    fb = b + 32768;

    if (fa < 32768 && fb < 32768) {
        fz = (fa * fb) / 32768;
    } else {
        fz = (2 * (fa + fb)) - ((fa * fb) / 32768) - 65536;
    }

    z = fz - 32768;
    return z;
}


extern "C" {
    void __ISR(_TIMER_4_VECTOR, IPL5) _T4Interrupt(void)
    {
        if (internalPWMAudioPointer) {
            internalPWMAudioPointer->generateAudio();
        }
        IFS0bits.T4IF = 0;
    }
}

