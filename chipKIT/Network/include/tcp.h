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

#ifndef _TCP_H
#define _TCP_H

#include "include/settings.h"
#include "Print.h"

#define TCP_CLOSED 0
#define TCP_LISTEN 1
#define TCP_SYNSENT 2
#define TCP_SYNRECEIVED 3
#define TCP_ESTABLISHED 4
#define TCP_FINWAIT1 5
#define TCP_FINWAIT2 6
#define TCP_CLOSEWAIT 7
#define TCP_CLOSING 8
#define TCP_LASTACK 9
#define TCP_TIMEWAIT 10

#define TCP_SYN 0x0001
#define TCP_ACK 0x0002
#define TCP_URG 0x0004
#define TCP_PSH 0x0008
#define TCP_RST 0x0010
#define TCP_FIN 0x0020

struct tcp_packet {
	unsigned short sourcePort;
	unsigned short destPort;
	uint32_t sequence;
	uint32_t acknowledgement;
	unsigned res1:4;
	unsigned offset:4;
	unsigned fin:1;
	unsigned syn:1;
	unsigned rst:1;
	unsigned psh:1;
	unsigned ack:1;
	unsigned urg:1;
	unsigned res2:2;
	unsigned short window;
	unsigned short checksum;
	unsigned short urgentPointer;
} __attribute__((packed));

class TCP : public Print{
	public:
		unsigned char rx[TCP_BUF_SIZE];
		unsigned char tx[TCP_BUF_SIZE];
		unsigned short rx_tail;
		unsigned short rx_head;
		unsigned short tx_tail;
		unsigned short tx_head;

		volatile unsigned long iss;	// Transmission Initial Send Sequence number

		volatile unsigned long tx_una;	// Transmission Unacknowledged Sequence
		volatile unsigned long tx_nxt;	// Transmission Next Sequence Number
		volatile unsigned long tx_wnd;	// Transmission Window
		volatile unsigned long tx_up;	// Transmission Urgent Pointer
		volatile unsigned long tx_wl1;	// Segment Sequence Number for last window update
		volatile unsigned long tx_wl2;	// Segment acknowledgement number for last window update
		volatile unsigned short tx_mss;

		volatile unsigned long irs;	// Initial receive sequence number

		volatile unsigned long rx_nxt;	// Next receive sequence number
		volatile unsigned long rx_wnd;	// Receive windows size
		volatile unsigned long rx_urg;	// Receive urgent pointer

		volatile unsigned short localPort;
		volatile unsigned short peerPort;
		volatile unsigned short interface;
		volatile unsigned long localAddress;
		volatile unsigned long peerAddress;
		volatile unsigned char state;
	public:
		void sendSyn();
		void sendAck();
		void sendFin();
		void sendRst(unsigned long ack);
		void sendSynAck();
		void sendFinAck();
		void sendRstAck(unsigned long ack);
		short waitSynAck();
		short sendPacket(unsigned char *data, unsigned short len, unsigned short flags);
                virtual void write(unsigned char);
		unsigned char socketMatch(unsigned short lp, unsigned short pp, unsigned long pa);
		void consumePacket(struct tcp_packet *p,unsigned short len);
		void actionClose();
		unsigned short available();
		unsigned short txQueueSize();
		short read(unsigned char *, unsigned short);
		short readln(char *, unsigned short);
		void dispatchPacket();
		void flush();
		void parseOptions(struct tcp_packet *p);
};

#endif
