/********************************************************************************
 * Copyright (c) 2012, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies, 
 * either expressed or implied, of the FreeBSD Project.
 ********************************************************************************/

#ifndef _GLCD_H
#define _GLCD_H

#include "Print.h"

#define GLCD_CMD_OFF         0b00111110
#define GLCD_CMD_ON          0b00111111
#define GLCD_CMD_SET_Y       0b01000000
#define GLCD_CMD_SET_PAGE    0b10111000
#define GLCD_CMD_START       0b11000000

#define GLCD_STAT_BUSY   0b10000000
#define GLCD_STAT_ONOFF  0b00100000
#define GLCD_STAT_RESET  0b00010000

struct iface {
	unsigned int di;
	unsigned int rw;
	unsigned int e;
	unsigned int db0;
	unsigned int db1;
	unsigned int db2;
	unsigned int db3;
	unsigned int db4;
	unsigned int db5;
	unsigned int db6;
	unsigned int db7;
	unsigned int cs1;
	unsigned int cs2;
	unsigned int res;
};

class _GLCD : public Print {
	public:
		struct iface pins;
		unsigned char screen[9][128];
		unsigned char x;
		unsigned char y;
		unsigned char gx;
		unsigned char gy;

	public:
		void begin( unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
		void init();
		void command(unsigned char, unsigned char);
		void data(unsigned char, unsigned char);
		void send(unsigned char, unsigned char);
		virtual void write(uint8_t);
		void lcd_delay();

		void set_page(unsigned char, unsigned char);
		void set_y(unsigned char, unsigned char);
		void update();

		void cls();
		void setPixel(unsigned char, unsigned char);
		void clearPixel(unsigned char, unsigned char);
		void scrollUp();
		void draw(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
		void setCursor(unsigned char, unsigned char);
		void setGCursor(unsigned char, unsigned char);
		void gChar(const char);
		void gText(const char *);
		void hScale(unsigned char, unsigned char, unsigned char, unsigned char, unsigned long, unsigned long);
};

extern _GLCD GLCD;

#endif
