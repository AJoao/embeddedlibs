#include <WProgram.h>
#include <DAC.h>

#include <p32xxxx.h>
#include <sys/attribs.h>

void _DAC::begin(unsigned long freq)
{

    T1CONbits.TCKPS = 0b111; // 1:4
    PR1 = (80000000 / 256) / freq;
    IPC1bits.T1IP = 3;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    T1CONbits.TON = 1;

    // Set the SPI interface up so all we need do is place a word in the buffer
    // and the hardware does all the rest for us.
//    OpenSPI2(FRAME_ENABLE_ON | FRAME_SYNC_OUTPUT | MASTER_ENABLE_ON | SEC_PRESCAL_1_1 | PRI_PRESCAL_1_1 | SPI_MODE16_ON | SPI_CKE_ON | SPI_SMP_ON, SPI_ENABLE );

    SPI2CONbits.MSTEN = 1;
    SPI2CONbits.FRMEN = 1;
    SPI2CONbits.FRMSYNC = 0;
    SPI2CONbits.FRMPOL = 0;
    SPI2CONbits.MODE16 = 1;
    SPI2CONbits.FRMSYPW = 1;
    SPI2CONbits.FRMCNT = 0;
    SPI2CONbits.SPIFE = 1;
    SPI2CONbits.CKE = 1;
    SPI2CONbits.SMP = 1;
    SPI2BRG = 15;
    SPI2CONbits.ON = 1;
}

void _DAC::sendSample()
{
    register int val = 0;
    register int i;
    register struct sample *s;
    static int lastsample = 0;

    for (i=0; i<MAX_SAMPLES; i++) {
        s = &(this->samples[i]);
        if (s->sample != NULL) {
            if (s->delay > 0) {
                s->delay--;
            } else {
                val += s->sample[s->pos];
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

    val = val >> 2;
    if (val < -32768) {
        val = -32768;
    }
    if (val > 32767) {
        val = 32767;
    }

    if (lastsample == val) {
        return;
    }

    lastsample = val;

    val = val + 32768;
    val = val >> 4;
    val = val & 0x0FFF;
    val = val | 0x3000;
    SPI2BUF = val;
}

void _DAC::queueSample(const short *sample, int length, int delay, int speed)
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
            return;
        }
    }
}

void _DAC::queueSingleSample(const short *sample, int length, int delay, int speed)
{
    int i;
    int found = 0;
    for (i=0; i<MAX_SAMPLES; i++) {
        if (this->samples[i].sample == sample) {
            found = 1;
        }
    }
    if (found == 0) {
        queueSample(sample, length, delay, speed);
    }
}


extern "C" {
    void __ISR(_TIMER_1_VECTOR, ipl3) _T1Interrupt(void)
    {
        DAC.sendSample();
        IFS0bits.T1IF = 0;
    }
};

_DAC DAC;
