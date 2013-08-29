#ifndef _PWMAUDIO_H
#define _PWMAUDIO_H

#include <WProgram.h>

#define USE_TIMER 4
#define MAX_SAMPLES 28
#define AUDIO_LOG2 12
#define AUDIO_BITS (1 << AUDIO_LOG2)

#define _BN_TIMER_OC    0

#define _TIMER_OC1      (1 << _BN_TIMER_OC)
#define _TIMER_OC2      (2 << _BN_TIMER_OC)
#define _TIMER_OC3      (3 << _BN_TIMER_OC)
#define _TIMER_OC4      (4 << _BN_TIMER_OC)
#define _TIMER_OC5      (5 << _BN_TIMER_OC)


struct sample {
    short * sample;
    int len;
    int delay;
    int pos;
    int speed;
    int spos;
    int volume;
};

class PWMAudio
{
    private:
        struct sample samples[MAX_SAMPLES];
        unsigned char audioPin;
        unsigned char amplifierPin;
        unsigned int sampleRate;
        unsigned char ampInvert;

        int mix(int a, int b);

    public:
        void queueSample(const short *sample, int length, int delay, int speed);
        void queueSample(const short *sample, int length, int delay, int speed, int volume);
        void queueSingleSample(const short *sample, int length, int delay, int speed);
        void queueSingleSample(const short *sample, int length, int delay, int speed, int volume);

        void ampOn();
        void ampOff();

        PWMAudio(unsigned char pin, unsigned int sfreq);
        PWMAudio(unsigned char pin, unsigned int sfreq, unsigned char amp);
        void generateAudio();
        void begin();
        void invertAmpTrigger();
};

#endif

