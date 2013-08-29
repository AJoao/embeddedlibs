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

#ifndef _NETWORK_H
#define _NETWORK_H

#include <DSPI.h>
#include "include/settings.h"
#include "include/arp.h"
#include "include/ethernet.h"
#include "include/circular.h"
#include "include/ENC28J60.h"
#include "include/errors.h"
#include "include/ip.h"
#include "include/icmp.h"
#include "include/udp.h"
#include "include/tcp.h"
#include "include/tcpclient.h"
#include "include/tcpserver.h"
#include "include/dns.h"
#include "include/httpd.h"

#define htons(x) (((x & 0xFF) << 8) | ((x & 0xFF00) >> 8))
#define ntohs(x) (((x & 0xFF) << 8) | ((x & 0xFF00) >> 8))

#define htonl(x) (((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24))
#define ntohl(x) (((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24))

class _Network {
	public:
		ENC28J60 *interfaces[MAX_INTERFACES];
		struct arp arp[MAX_ARP];
		UDP *udp_ports[MAX_PORTS];
		TCPClient *tcp_clients[MAX_PORTS];
		TCPServer *tcp_servers[MAX_PORTS];
		unsigned long defaultRouter;

	public:
		_Network();
		short addInterface(ENC28J60 *);
		void tick();
		void addOrUpdateARP(unsigned char mac[6], unsigned long ip, unsigned short iface);
		short addPort(UDP *);
		short addPort(TCPClient *);
		short addPort(TCPServer *);
		short getInterfaceByIP(unsigned long);
		short getDefaultInterface();
		void setDefaultRouter(unsigned long);
		short getARP(unsigned long, unsigned char *, short);
};

extern _Network Network;

#define CS_IP 0
#define CS_UDP 1
#define CS_TCP 2

extern unsigned short checksum16(unsigned char *buf, unsigned short len, unsigned char type);
//extern unsigned short checksum16(unsigned char *data, unsigned short len);
//extern unsigned short checksum16(unsigned char *data, unsigned short len, unsigned short ival);
extern unsigned long quad2ip(unsigned char, unsigned char, unsigned char, unsigned char);


#endif
