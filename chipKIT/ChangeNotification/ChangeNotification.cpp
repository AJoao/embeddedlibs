#include <plib.h>
#include "ChangeNotification.h"

struct cn_state cn_states[NUM_CN];

const struct cn_setting cn_settings[NUM_CN] = {
	{ 14, &TRISC, &PORTC }, // CN0  - RC14
	{ 13, &TRISC, &PORTC }, // CN1  - RC13
	{  0, &TRISB, &PORTB }, // CN2  - RB0
	{  1, &TRISB, &PORTB }, // CN3  - RB1
	{  2, &TRISB, &PORTB }, // CN4  - RB2
	{  3, &TRISB, &PORTB }, // CN5  - RB3
	{  4, &TRISB, &PORTB }, // CN6  - RB4
	{  5, &TRISB, &PORTB }, // CN7  - RB5
	{  6, &TRISG, &PORTG }, // CN8  - RG6
	{  7, &TRISG, &PORTG }, // CN9  - RG7
	{  8, &TRISG, &PORTG }, // CN10 - RG8
	{  9, &TRISG, &PORTG }, // CN11 - RG9
	{ 15, &TRISB, &PORTB }, // CN12 - RB15
	{  4, &TRISD, &PORTD }, // CN13 - RD4
	{  5, &TRISD, &PORTD }, // CN14 - RD5
	{  6, &TRISD, &PORTD }, // CN15 - RD6
	{  7, &TRISD, &PORTD }, // CN16 - RD7
	{  4, &TRISF, &PORTF }, // CN17 - RF4
	{  5, &TRISF, &PORTF }, // CN18 - RF5
	{ 13, &TRISD, &PORTD }, // CN19 - RD13
	{ 14, &TRISD, &PORTD }, // CN20 - RD14
	{ 15, &TRISD, &PORTD }, // CN21 - RD15
};

void attachInterrupt(struct cn cn, void (*function)(), unsigned char type)
{
	int s;

	switch(type)
	{
		case RISING:
			cn_states[cn.num].rising = function;
			break;
		case FALLING:
			cn_states[cn.num].falling = function;
			break;
		case CHANGE:
			cn_states[cn.num].change = function;
			break;
	}


	*(cn_settings[cn.num].TRIS) |= (1<<cn_settings[cn.num].pin);
	CNEN |= 1<<cn.num;
	s = *(cn_settings[cn.num].PORT) & (1<<cn_settings[cn.num].pin);
	cn_states[cn.num].current = cn_states[cn.num].previous = s == 0 ? LOW : HIGH;
	IFS1bits.CNIF=0;
	IEC1bits.CNIE=1;
	IPC6bits.CNIP	=	_CN_IPL_IPC;
	IPC6bits.CNIS	=	_CN_SPL_IPC;
	CNCONbits.ON	=	1;
	CNCONbits.SIDL	=	0;
}

void detachInterrupt(struct cn cn)
{
	detachInterrupt(cn,RISING);
	detachInterrupt(cn,FALLING);
	detachInterrupt(cn,CHANGE);
}

void detachInterrupt(struct cn cn, char type)
{
	switch(type)
	{
		case RISING:
			cn_states[cn.num].rising = NULL;
			break;
		case FALLING:
			cn_states[cn.num].falling = NULL;
			break;
		case CHANGE:
			cn_states[cn.num].change = NULL;
			break;
	}

	if((cn_states[cn.num].rising == NULL) &&
		(cn_states[cn.num].falling == NULL) &&
		(cn_states[cn.num].change == NULL))
			CNEN &= ~(1<<cn.num);
}

extern "C" {
	void __ISR(_CHANGE_NOTICE_VECTOR, _CN_IPL_ISR) cn_isr()
	{
		for(unsigned char i=0; i<NUM_CN; i++)
		{
			int s = *(cn_settings[i].PORT) & (1<<cn_settings[i].pin);
			unsigned char onoff = s == 0 ? LOW : HIGH;
			if(cn_states[i].current != onoff)
			{
				cn_states[i].previous = cn_states[i].current;
				cn_states[i].current = onoff;

				if(((cn_states[i].falling!=NULL) || (cn_states[i].change!=NULL))
					 && (cn_states[i].previous == HIGH) && (cn_states[i].current == LOW))
				{
					if(cn_states[i].falling != NULL)
						cn_states[i].falling();
					if(cn_states[i].change != NULL)
						cn_states[i].change();
				}

				if(((cn_states[i].rising!=NULL) || (cn_states[i].change!=NULL))
					 && (cn_states[i].previous == LOW) && (cn_states[i].current == HIGH))
				{
					if(cn_states[i].rising != NULL)
						cn_states[i].rising();
					if(cn_states[i].change != NULL)
						cn_states[i].change();
				}
			}
		}
		IFS1bits.CNIF=0;
	}
}
