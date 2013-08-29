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
 * either expressed or implied, of Majenko Technologies
 ********************************************************************************/

#include <plib.h>
#include <WProgram.h>
#include "Network.h"

TCPClient::TCPClient()
{
	this->tx_mss = 1440;
}

short TCPClient::connect(unsigned long ip, unsigned short port)
{
	short iface;

	this->tx_head = 0;
	this->tx_tail = 0;
	this->rx_head = 0;
	this->rx_tail = 0;

	this->iss = rand();
	this->tx_nxt = this->iss;
	this->localPort = (rand()&0xFFFF) | 0xF000;
	this->tx_mss = 1440;

	iface = Network.getInterfaceByIP(ip);
	if(iface<0)
	{
		iface = Network.getInterfaceByIP(Network.defaultRouter);
	}
	if(iface<0)
	{
		return -ENROUTE;
	}
	this->localAddress = Network.interfaces[iface]->ip;

	this->peerPort = port;
	this->peerAddress = ip;

	this->sendSyn();
        this->state = TCP_SYNSENT;
	if(this->waitSynAck()<0)
	{
		return -ETIME;
	}
	this->sendAck();
        this->state = TCP_ESTABLISHED;
	
	return 0;
}

void TCPClient::close()
{
	this->tx_head = 0;
	this->tx_tail = 0;
	this->rx_head = 0;
	this->rx_tail = 0;

	this->sendFinAck();
	this->state=TCP_FINWAIT1;
	while(this->state!=TCP_CLOSED);
}

void TCPClient::handleIncomingPacket(struct ip_packet *ip)
{
	unsigned short ipdoff;
	unsigned short ipdlen;
	unsigned char *ipdata;
	struct tcp_packet *tcp;

	ipdoff = ip->ihl << 2;
	ipdlen = ntohs(ip->tot_len) - ipdoff;
	ipdata = (unsigned char *)ip+ipdoff;
	tcp = (struct tcp_packet *)ipdata;

	this->tx_wnd = tcp->window;
	if(tcp->rst==1)
	{
		this->state = TCP_CLOSED;
	}

	if(tcp->syn==1 && tcp->ack==1)
	{
		if(this->state == TCP_SYNSENT)
		{
			this->irs = ntohl(tcp->sequence);
			this->rx_nxt = this->irs+1;
			this->state = TCP_SYNRECEIVED;
		} else if(this->state == TCP_SYNRECEIVED) {
			this->state = TCP_ESTABLISHED;
		}
	}

	if(tcp->fin==1 && tcp->ack==1)
	{
		if(this->state == TCP_ESTABLISHED || this->state == TCP_FINWAIT1)
		{
			this->actionClose();
		} else {
			this->sendRstAck(ntohl(tcp->acknowledgement));
		}
	}

	if(tcp->syn==0 && tcp->ack==1 && tcp->fin==0 && tcp->rst==0)
	{
		this->consumePacket(tcp,ipdlen);
	}
}
