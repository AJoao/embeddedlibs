#ifndef _DAC_H
#define _DAC_H

#define MAX_SAMPLES 28

struct sample {
    short * sample;
    int len;
    int delay;
    int pos;
    int speed;
    int spos;
};

class _DAC {
    private:
        struct sample samples[MAX_SAMPLES];

    public:
        void begin(unsigned long);
        void sendSample();
        void queueSample(const short *, int, int, int);
        void queueSingleSample(const short *, int, int, int);

};

extern _DAC DAC;
#endif
