#ifndef _CHANGENOTIFICATION_H
#define _CHANGENOTIFICATION_H

#include <WProgram.h>

struct cn {
	unsigned char num;
};

#define CN_0 (struct cn){0}
#define CN_1 (struct cn){1}
#define CN_2 (struct cn){2}
#define CN_3 (struct cn){3}
#define CN_4 (struct cn){4}
#define CN_5 (struct cn){5}
#define CN_6 (struct cn){6}
#define CN_7 (struct cn){7}
#define CN_8 (struct cn){8}
#define CN_9 (struct cn){9}
#define CN_10 (struct cn){10}
#define CN_11 (struct cn){11}
#define CN_12 (struct cn){12}
#define CN_13 (struct cn){13}
#define CN_14 (struct cn){14}
#define CN_15 (struct cn){15}
#define CN_16 (struct cn){16}
#define CN_17 (struct cn){17}
#define CN_18 (struct cn){18}
#define CN_19 (struct cn){19}
#define CN_20 (struct cn){20}
#define CN_21 (struct cn){21}

#define NUM_CN 22

struct cn_state {
	unsigned char previous;
	unsigned char current;
	void (*rising)();
	void (*falling)();
	void (*change)();
};

extern struct cn_state cn_states[NUM_CN];

struct cn_setting {
	unsigned char pin;
	volatile uint32_t *TRIS;
	volatile uint32_t *PORT;
};

void attachInterrupt(struct cn, void (*)(), unsigned char);
void detachInterrupt(struct cn);
void detachInterrupt(struct cn, unsigned char);

#endif
