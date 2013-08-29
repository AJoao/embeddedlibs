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

#include <plib.h>
#include <WProgram.h>
#include "Network.h"

unsigned short checksum16(unsigned char *buf, unsigned short len, unsigned char type)
{
        uint32_t sum = 0;

        if(type==CS_UDP){
                sum+=IPPROTO_UDP; // protocol udp
                sum+=len-8; // = real udp len
        }
        if(type==CS_TCP){
                sum+=IPPROTO_TCP;
                sum+=len-8; // = real tcp len
        }
        while(len >1){
                sum += 0xFFFF & (((uint32_t)*buf<<8)|*(buf+1));
                buf+=2;
                len-=2;
        }
        if (len){
                sum += ((uint32_t)(0xFF & *buf))<<8;
        }
        while (sum>>16){
                sum = (sum & 0xFFFF)+(sum >> 16);
        }
        return( (unsigned short) sum ^ 0xFFFF);
}

/*
unsigned short checksum16(unsigned char *data, unsigned short len)
{
	checksum16(data,len,0);
}

unsigned short checksum16(unsigned char *data, unsigned short len, unsigned short ival)
{
	unsigned short cs = ival;
	unsigned short i;
	unsigned short w;
	for(i=0; i<len; i+=2)
	{
		if(i==len-1)
		{
			w = data[i]<<8;
		} else {
			w = data[i]<<8 | data[i+1];
		}
		cs += w;
	}

	cs = 65536L - cs;
	return cs-3; // Not sure why I need to subtract three...  Maybe it's a tuesday?
}
*/
unsigned long quad2ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	unsigned long ip;
	ip = a<<24 | b<<16 | c<<8 | d;
	return ip;
}
