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

#include <WProgram.h>
#include "GLCD.h"

_GLCD GLCD;

const __attribute__((space(prog))) unsigned char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,// (space)
        0x00, 0x00, 0x5F, 0x00, 0x00,// !
        0x00, 0x07, 0x00, 0x07, 0x00,// "
        0x14, 0x7F, 0x14, 0x7F, 0x14,// #
        0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
        0x23, 0x13, 0x08, 0x64, 0x62,// %
        0x36, 0x49, 0x55, 0x22, 0x50,// &
        0x00, 0x05, 0x03, 0x00, 0x00,// '
        0x00, 0x1C, 0x22, 0x41, 0x00,// (
        0x00, 0x41, 0x22, 0x1C, 0x00,// )
        0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
        0x08, 0x08, 0x3E, 0x08, 0x08,// +
        0x00, 0x50, 0x30, 0x00, 0x00,// ,
        0x08, 0x08, 0x08, 0x08, 0x08,// -
        0x00, 0x60, 0x60, 0x00, 0x00,// .
        0x20, 0x10, 0x08, 0x04, 0x02,// /
        0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
        0x00, 0x42, 0x7F, 0x40, 0x00,// 1
        0x42, 0x61, 0x51, 0x49, 0x46,// 2
        0x21, 0x41, 0x45, 0x4B, 0x31,// 3
        0x18, 0x14, 0x12, 0x7F, 0x10,// 4
        0x27, 0x45, 0x45, 0x45, 0x39,// 5
        0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
        0x01, 0x71, 0x09, 0x05, 0x03,// 7
        0x36, 0x49, 0x49, 0x49, 0x36,// 8
        0x06, 0x49, 0x49, 0x29, 0x1E,// 9
        0x00, 0x36, 0x36, 0x00, 0x00,// :
        0x00, 0x56, 0x36, 0x00, 0x00,// ;
        0x00, 0x08, 0x14, 0x22, 0x41,// <
        0x14, 0x14, 0x14, 0x14, 0x14,// =
        0x41, 0x22, 0x14, 0x08, 0x00,// >
        0x02, 0x01, 0x51, 0x09, 0x06,// ?
        0x32, 0x49, 0x79, 0x41, 0x3E,// @
        0x7E, 0x11, 0x11, 0x11, 0x7E,// A
        0x7F, 0x49, 0x49, 0x49, 0x36,// B
        0x3E, 0x41, 0x41, 0x41, 0x22,// C
        0x7F, 0x41, 0x41, 0x22, 0x1C,// D
        0x7F, 0x49, 0x49, 0x49, 0x41,// E
        0x7F, 0x09, 0x09, 0x01, 0x01,// F
        0x3E, 0x41, 0x41, 0x51, 0x32,// G
        0x7F, 0x08, 0x08, 0x08, 0x7F,// H
        0x00, 0x41, 0x7F, 0x41, 0x00,// I
        0x20, 0x40, 0x41, 0x3F, 0x01,// J
        0x7F, 0x08, 0x14, 0x22, 0x41,// K
        0x7F, 0x40, 0x40, 0x40, 0x40,// L
        0x7F, 0x02, 0x04, 0x02, 0x7F,// M
        0x7F, 0x04, 0x08, 0x10, 0x7F,// N
        0x3E, 0x41, 0x41, 0x41, 0x3E,// O
        0x7F, 0x09, 0x09, 0x09, 0x06,// P
        0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
        0x7F, 0x09, 0x19, 0x29, 0x46,// R
        0x46, 0x49, 0x49, 0x49, 0x31,// S
        0x01, 0x01, 0x7F, 0x01, 0x01,// T
        0x3F, 0x40, 0x40, 0x40, 0x3F,// U
        0x1F, 0x20, 0x40, 0x20, 0x1F,// V
        0x7F, 0x20, 0x18, 0x20, 0x7F,// W
        0x63, 0x14, 0x08, 0x14, 0x63,// X
        0x03, 0x04, 0x78, 0x04, 0x03,// Y
        0x61, 0x51, 0x49, 0x45, 0x43,// Z
        0x00, 0x00, 0x7F, 0x41, 0x41,// [
        0x02, 0x04, 0x08, 0x10, 0x20,// "\"
        0x41, 0x41, 0x7F, 0x00, 0x00,// ]
        0x04, 0x02, 0x01, 0x02, 0x04,// ^
        0x40, 0x40, 0x40, 0x40, 0x40,// _
        0x00, 0x01, 0x02, 0x04, 0x00,// `
        0x20, 0x54, 0x54, 0x54, 0x78,// a
        0x7F, 0x48, 0x44, 0x44, 0x38,// b
        0x38, 0x44, 0x44, 0x44, 0x20,// c
        0x38, 0x44, 0x44, 0x48, 0x7F,// d
        0x38, 0x54, 0x54, 0x54, 0x18,// e
        0x08, 0x7E, 0x09, 0x01, 0x02,// f
        0x08, 0x14, 0x54, 0x54, 0x3C,// g
        0x7F, 0x08, 0x04, 0x04, 0x78,// h
        0x00, 0x44, 0x7D, 0x40, 0x00,// i
        0x20, 0x40, 0x44, 0x3D, 0x00,// j
        0x00, 0x7F, 0x10, 0x28, 0x44,// k
        0x00, 0x41, 0x7F, 0x40, 0x00,// l
        0x7C, 0x04, 0x18, 0x04, 0x78,// m
        0x7C, 0x08, 0x04, 0x04, 0x78,// n
        0x38, 0x44, 0x44, 0x44, 0x38,// o
        0x7C, 0x14, 0x14, 0x14, 0x08,// p
        0x08, 0x14, 0x14, 0x18, 0x7C,// q
        0x7C, 0x08, 0x04, 0x04, 0x08,// r
        0x48, 0x54, 0x54, 0x54, 0x20,// s
        0x04, 0x3F, 0x44, 0x40, 0x20,// t
        0x3C, 0x40, 0x40, 0x20, 0x7C,// u
        0x1C, 0x20, 0x40, 0x20, 0x1C,// v
        0x3C, 0x40, 0x30, 0x40, 0x3C,// w
        0x44, 0x28, 0x10, 0x28, 0x44,// x
        0x0C, 0x50, 0x50, 0x50, 0x3C,// y
        0x44, 0x64, 0x54, 0x4C, 0x44,// z
        0x00, 0x08, 0x36, 0x41, 0x00,// {
        0x00, 0x00, 0x7F, 0x00, 0x00,// |
        0x00, 0x41, 0x36, 0x08, 0x00,// }
        0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
        0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};

void _GLCD::begin(
	unsigned int di, unsigned int rw, unsigned int e, unsigned int db0,
	unsigned int db1, unsigned int db2, unsigned int db3, unsigned int db4,
	unsigned int db5, unsigned int db6, unsigned int db7, unsigned int cs1,
	unsigned int cs2, unsigned int res)
{
	this->pins.di = di;
	this->pins.rw = rw;
	this->pins.e = e;
	this->pins.db0 = db0;
	this->pins.db1 = db1;
	this->pins.db2 = db2;
	this->pins.db3 = db3;
	this->pins.db4 = db4;
	this->pins.db5 = db5;
	this->pins.db6 = db6;
	this->pins.db7 = db7;
	this->pins.cs1 = cs1;
	this->pins.cs2 = cs2;
	this->pins.res = res;

	pinMode(this->pins.di,OUTPUT);
	pinMode(this->pins.rw,OUTPUT);
	pinMode(this->pins.e,OUTPUT);
	pinMode(this->pins.db0,OUTPUT);
	pinMode(this->pins.db1,OUTPUT);
	pinMode(this->pins.db2,OUTPUT);
	pinMode(this->pins.db3,OUTPUT);
	pinMode(this->pins.db4,OUTPUT);
	pinMode(this->pins.db5,OUTPUT);
	pinMode(this->pins.db6,OUTPUT);
	pinMode(this->pins.db7,OUTPUT);
	pinMode(this->pins.cs1,OUTPUT);
	pinMode(this->pins.cs2,OUTPUT);
	pinMode(this->pins.res,OUTPUT);

	this->init();
}

void _GLCD::init()
{
	digitalWrite(this->pins.res,LOW);

	digitalWrite(this->pins.e,LOW);
	digitalWrite(this->pins.cs1,LOW);
	digitalWrite(this->pins.cs2,LOW);

	delay(1);
	digitalWrite(this->pins.res,HIGH);

	delay(1);
	this->command(0,GLCD_CMD_ON);
	this->command(1,GLCD_CMD_ON);
	this->command(0,GLCD_CMD_START);
	this->command(1,GLCD_CMD_START);

	this->cls();
	this->update();
}

void _GLCD::command(unsigned char chip, unsigned char cmd)
{
	digitalWrite(this->pins.di,LOW);
	this->send(chip,cmd);
}

void _GLCD::data(unsigned char chip, unsigned char dat)
{
	digitalWrite(this->pins.di,HIGH);
	this->send(chip,dat);
}

void _GLCD::send(unsigned char chip, unsigned char val)
{
	digitalWrite(this->pins.rw,LOW);
	pinMode(this->pins.db0,OUTPUT);
	pinMode(this->pins.db1,OUTPUT);
	pinMode(this->pins.db2,OUTPUT);
	pinMode(this->pins.db3,OUTPUT);
	pinMode(this->pins.db4,OUTPUT);
	pinMode(this->pins.db5,OUTPUT);
	pinMode(this->pins.db6,OUTPUT);
	pinMode(this->pins.db7,OUTPUT);

	digitalWrite(chip==0 ? this->pins.cs1 : this->pins.cs2, HIGH);

	digitalWrite(this->pins.db0, val & 0b00000001 ? HIGH : LOW);
	digitalWrite(this->pins.db1, val & 0b00000010 ? HIGH : LOW);
	digitalWrite(this->pins.db2, val & 0b00000100 ? HIGH : LOW);
	digitalWrite(this->pins.db3, val & 0b00001000 ? HIGH : LOW);
	digitalWrite(this->pins.db4, val & 0b00010000 ? HIGH : LOW);
	digitalWrite(this->pins.db5, val & 0b00100000 ? HIGH : LOW);
	digitalWrite(this->pins.db6, val & 0b01000000 ? HIGH : LOW);
	digitalWrite(this->pins.db7, val & 0b10000000 ? HIGH : LOW);

	digitalWrite(this->pins.e,HIGH);
	this->lcd_delay();
	digitalWrite(this->pins.e,LOW);
	this->lcd_delay();

	digitalWrite(chip==0 ? this->pins.cs1 : this->pins.cs2, LOW);
}

void _GLCD::lcd_delay()
{
//	delay(1);
	delayMicroseconds(1);
//	unsigned long c;
//	for(c=0; c<200; c++)
//		volatile asm("nop");
}

void _GLCD::set_page(unsigned char chip, unsigned char page)
{
	this->command(chip,GLCD_CMD_SET_PAGE | (page & 0x07));
}

void _GLCD::set_y(unsigned char chip, unsigned char y)
{
	this->command(chip,GLCD_CMD_SET_Y | (y & 63));
}

void _GLCD::update()
{
	unsigned char x,y;
	for(y=0; y<8; y++)
	{
		this->set_page(0,y);
		this->set_page(1,y);
		this->set_y(0,0);
		this->set_y(1,0);
		for(x=0; x<64; x++)
		{
			this->data(0,this->screen[y][x]);
			this->data(1,this->screen[y][x+64]);
		}
	}
}

void _GLCD::cls()
{
	unsigned char x,y;
	for(y=0; y<8; y++)
	{
		for(x=0; x<128; x++)
		{
			screen[y][x] = 0;
		}
	}
	this->x = 0;
	this->y = 0;
}

void _GLCD::setPixel(unsigned char x, unsigned char y)
{
	unsigned char row;
	unsigned char pixel;
	unsigned char mask;

	if(x>127) return;
 	if(y>71) return;

	row = y>>3;
	pixel = y & 0x07;
	mask = 1<<pixel;
	this->screen[row][x] |= mask;
}

void _GLCD::clearPixel(unsigned char x, unsigned char y)
{
	unsigned char row;
	unsigned char pixel;
	unsigned char mask;

	if(x>127) return;
 	if(y>63) return;

	row = y>>3;
	pixel = y & 0x07;
	mask = 1<<pixel;
	this->screen[row][x] &= ~mask;
}

void _GLCD::write(uint8_t c)
{
	unsigned int cpos;
	if(c=='\r')
	{
		this->x = 0;
		return;
	}

	if(c=='\n')
	{
		this->y++;
		if(this->y>7)
		{
			this->y--;
			for(cpos=0; cpos<8; cpos++)
				this->scrollUp();
		}       
		return;
	}
	if(c==0x08)
	{
		if(this->x>0)
		{
			this->x-=6;
		} else {
			if(this->y>0)
			{
				this->x = 127-7;
				this->y--;
			}
		}
		return;
	}
	if(c==0x07)
	{
		return;
	}
	if(c==0x0C)
	{
		this->cls();
		this->update();
		return;
	}
	if((c<' ') || (c>'z'))
		return;
	if(this->x > 127-6)
	{
		this->x=0;
		this->y++;
		if(this->y>7)
		{
			this->y--;
			for(cpos=0; cpos<8; cpos++)
				this->scrollUp();
		}
	}
	cpos = (c - ' ') * 5;
	this->screen[this->y][this->x++] = font[cpos++];
	this->screen[this->y][this->x++] = font[cpos++];
	this->screen[this->y][this->x++] = font[cpos++];
	this->screen[this->y][this->x++] = font[cpos++];
	this->screen[this->y][this->x++] = font[cpos++];
	this->screen[this->y][this->x++] = 0;
}

void _GLCD::scrollUp()
{
        unsigned char x,y;
        for(y=0; y<9; y++)
        {
                for(x=0; x<128; x++)
                {
                        this->screen[y][x] = this->screen[y][x]>>1;
                        if(y<8)
                                this->screen[y][x] |= (this->screen[y+1][x]<<7);
                }
        }
}

void _GLCD::draw(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2, unsigned char ink)
{
        int w, h;
        int dx1=0, dy1=0, dx2=0, dy2=0;
        int longest, shortest;
        int numerator;
        int i;

	w = (int)x2 - (int)x;
        h = (int)y2 - (int)y;

        if(w<0)
        {
                dx1 = -1;
                dx2 = -1;
        }
        else if(w>0)
        {
                dx1 = 1;
                dx2 = 1;
        }

        if(h<0)
                dy1 = -1;
        else if(h>0)
                dy1 = 1;

        longest = abs(w);
        shortest = abs(h);
        if(!(longest>shortest))
        {
                longest = abs(h);
                shortest = abs(w);
                if(h<0)
                        dy2 = -1;
                else if(h>0)
                        dy2 = 1;
                dx2=0;
        }

        numerator = longest >> 1;
        for(i=0; i<=longest; i++)
        {
                if(ink==1)
                    this->setPixel(x,y);
                else
                    this->clearPixel(x,y);
                numerator += shortest;
                if(!(numerator<longest))
                {
                        numerator -= longest;
                        x += dx1;
                        y += dy1;
                } else {
                        x += dx2;
                        y += dy2;
                }
        }
}

void _GLCD::setCursor(unsigned char x,unsigned char y)
{
	this->x = x;
	this->y = y;
}

void _GLCD::setGCursor(unsigned char x,unsigned char y)
{
	this->gx = x;
	this->gy = y;
}

void _GLCD::gText(const char *text)
{
	const char *p;
	for(p=text; *p; p++)
		this->gChar(*p);
}

void _GLCD::gChar(const char c)
{
	int cpos;
	unsigned char cdat;
        cpos = (c - ' ') * 5;
	for(int dx=0; dx<5; dx++)
	{
		cdat = font[cpos];
		for(int dy=0; dy<8; dy++)
		{
			if(cdat & 1)
			{
				setPixel(this->gx,this->gy+dy);
			}
			cdat = cdat >> 1;
		}

		cpos++;
		this->gx++;
	}
	this->gx++;
}

void _GLCD::hScale(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned long maxvalue, unsigned long value)
{
	unsigned char i;
	unsigned char bw;

	this->draw(x,y,x+w,y,1);
	this->draw(x,y,x,y+h,1);
	this->draw(x,y+h,x+w,y+h,1);
	this->draw(x+w,y,x+w,y+h,1);

	bw = (value * (w-4)) / maxvalue;
	for(i=0; i<h-3; i++)
	{
		this->draw(x+2,y+2+i,x+2+bw,y+2+i,1);
	}
}
