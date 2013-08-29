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

void TCP::sendSyn()
{
	this->sendPacket((unsigned char *)"",0,TCP_SYN);
}

void TCP::sendAck()
{
	this->sendPacket((unsigned char *)"",0,TCP_ACK);
}

void TCP::sendFin()
{
	this->sendPacket((unsigned char *)"",0,TCP_FIN);
}

void TCP::sendRst(unsigned long ack)
{
	this->tx_nxt = ack;
	this->sendPacket((unsigned char *)"",0,TCP_RST);
	this->state = TCP_CLOSED;
}

void TCP::sendSynAck()
{
	this->sendPacket((unsigned char *)"",0,TCP_SYN | TCP_ACK);
}

void TCP::sendFinAck()
{
	this->sendPacket((unsigned char *)"",0,TCP_FIN | TCP_ACK);
}

void TCP::sendRstAck(unsigned long ack)
{
	this->tx_nxt = ack;
	this->sendPacket((unsigned char *)"",0,TCP_RST | TCP_ACK);
	this->state = TCP_CLOSED;
}

short TCP::waitSynAck()
{
	unsigned long started = millis();
	while(millis() - started < 1000)
	{
		if(state == TCP_CLOSED)
		{
			return -ENCONN;
		}
		if(state == TCP_SYNRECEIVED)
		{
			return 0;
		}
	}
	return -ETIME;
}

short TCP::sendPacket(unsigned char *data, unsigned short len, unsigned short flags)
{
        short interface;
        struct ethernet_packet eth;
        unsigned long ipAddress = this->peerAddress;
        short rv;
	unsigned char extra = 0;
        struct ip_packet *ip;
        unsigned char *ipdata;
        struct tcp_packet *tcp;
        unsigned char *tcpdata;

        interface = Network.getInterfaceByIP(this->peerAddress);
        if(interface<0)
        {
                ipAddress = Network.defaultRouter;
                interface = Network.getDefaultInterface();
        }
        if(interface<0)
        {
                return -ENROUTE;
        }

        memset((char *)&eth,0,sizeof(struct ethernet_packet));
        eth.header.ether_type=htons(ETHERTYPE_IP);
        memcpy(eth.header.ether_shost,Network.interfaces[interface]->mac,6);
        rv = Network.getARP(ipAddress,(unsigned char *)eth.header.ether_dhost,interface);
        if(rv < 0)
        {
                return -ENARP;
        }
        ip = (struct ip_packet *)(&eth.payload);
        ip->protocol = IPPROTO_TCP;
        ip->ttl = 64;
        ip->tos = 0;
        ip->id = htons(Network.interfaces[interface]->id());
        ip->flags = 0b010;
        ip->frag_off = 0;
        ip->frag_offh = 0;
        ip->saddr[3] = Network.interfaces[interface]->ip & 0xFF;
        ip->saddr[2] = Network.interfaces[interface]->ip>>8 & 0xFF;
        ip->saddr[1] = Network.interfaces[interface]->ip>>16 & 0xFF;
        ip->saddr[0] = Network.interfaces[interface]->ip>>24 & 0xFF;
        ip->daddr[3] = this->peerAddress & 0xFF;
        ip->daddr[2] = this->peerAddress>>8 & 0xFF;
        ip->daddr[1] = this->peerAddress>>16 & 0xFF;
        ip->daddr[0] = this->peerAddress>>24 & 0xFF;
        ip->version = 4;
        ip->ihl = 5;
        ip->tot_len = htons(sizeof(struct tcp_packet) + 20 + len);
        ip->check = 0;
        ipdata = (unsigned char *)ip + 20;
        tcp = (struct tcp_packet *)ipdata;
        tcp->sourcePort = htons(this->localPort);
        tcp->destPort = htons(this->peerPort);
        tcp->checksum = 0;
	tcp->offset = 5;
	tcp->sequence = htonl(this->tx_nxt);
	tcp->acknowledgement = htonl(this->rx_nxt);
	tcp->window = htons(TCP_BUF_SIZE - this->available());
	
	tcp->syn = flags&TCP_SYN ? 1 : 0;
	extra   += flags&TCP_SYN ? 1 : 0;

	tcp->ack = flags&TCP_ACK ? 1 : 0;
	tcp->psh = flags&TCP_PSH ? 1 : 0;

	tcp->fin = flags&TCP_FIN ? 1 : 0;
	extra   += flags&TCP_FIN ? 1 : 0;

	tcp->rst = flags&TCP_RST ? 1 : 0;
	tcp->urg = flags&TCP_URG ? 1 : 0;

        tcpdata = (unsigned char *)tcp+sizeof(struct tcp_packet);
        memcpy(tcpdata,data,len);

        tcp->checksum = htons(checksum16((unsigned char *)&ip->saddr[0],len+20+8,CS_TCP));

        ip->check = htons(checksum16((unsigned char *)ip,20,CS_IP));
        Network.interfaces[interface]->queueTX((unsigned char *)&eth,sizeof(struct ether_header)+20+sizeof(struct tcp_packet)+len);
	this->tx_nxt+=len;
	this->tx_nxt+=extra;
        return len;
}

void TCP::write(unsigned char v)
{
	this->tx[this->tx_head++] = v;
	if(tx_head==TCP_BUF_SIZE)
		tx_head=0;
	if((this->txQueueSize()>=this->tx_mss) || (this->txQueueSize()>=this->tx_wnd))
	{
		this->dispatchPacket();
	}
}

void TCP::dispatchPacket()
{
	unsigned char buf[1500];
	unsigned short i;
	unsigned short len;

	len = this->txQueueSize();
	if(len > this->tx_mss)
		len = this->tx_mss;
	if(len > this->tx_wnd)
		len = this->tx_wnd;

	for(i=0; i<len; i++)
	{
		buf[i] = this->tx[this->tx_tail++];
		if(this->tx_tail==TCP_BUF_SIZE)
			this->tx_tail=0;
	}
	this->sendPacket(buf,len,TCP_ACK);
}

unsigned char TCP::socketMatch(unsigned short lp, unsigned short pp, unsigned long pa)
{
	if(lp != this->localPort)
		return 0;
	if(pp != this->peerPort)
		return 0;
	if(pa != this->peerAddress)
		return 0;
	return 1;
}

void TCP::consumePacket(struct tcp_packet *p,unsigned short len)
{
	unsigned char *tcpdata;
	unsigned short datoff;
	unsigned short i;

	datoff = p->offset*4;
	tcpdata = (unsigned char *)p + datoff;
	len -= datoff;
	this->rx_nxt+=len;
	for(i=0; i<len; i++)
	{
		this->rx[this->rx_head++] = tcpdata[i];
		if(this->rx_head == TCP_BUF_SIZE)
		{
			this->rx_head = 0;
		}
	}

	this->sendAck();
}

void TCP::actionClose()
{
	this->sendFinAck();
	this->state=TCP_CLOSEWAIT;
}

unsigned short TCP::txQueueSize()
{
	return (TCP_BUF_SIZE + this->tx_head - this->tx_tail) % TCP_BUF_SIZE;
}

unsigned short TCP::available()
{
	return (TCP_BUF_SIZE + this->rx_head - this->rx_tail) % TCP_BUF_SIZE;
}

short TCP::read(unsigned char *buf, unsigned short len)
{
	unsigned short i;
	if(this->available()==0)
	{
		return 0;
	}

	if(len > this->available())
	{
		len = this->available();
	}

	for(i=0; i<len; i++)
	{
		buf[i] = this->rx[this->rx_tail++];
		buf[i+1]=0;
		if(this->rx_tail==TCP_BUF_SIZE)
			this->rx_tail = 0;
	}
	return len;
}

short TCP::readln(char *buf, unsigned short len)
{
	unsigned short i;
	unsigned long timeout;
	unsigned char in[2];
	for(i=0; i<len; i++)
	{
		timeout = millis();
		while((this->available()==0) && (millis()-timeout<3000));
		if(millis()-timeout>=3000)
			return -1;
		this->read(in,1);
		if(in[0]=='\n')
		{
			if(i>0)
			{
				if(buf[i-1]=='\r')
				{
					buf[i-1]=0;
					i--;
				}
			}
			return i;
		}
		buf[i] = in[0];
		buf[i+1] = 0;
	}
	return len;
}

void TCP::flush()
{
	while(this->txQueueSize()>0)
	{
		this->dispatchPacket();
	}
}

void TCP::parseOptions(struct tcp_packet *p)
{
	unsigned char *ptr;
	unsigned char *optend;
	optend = (unsigned char *)p + (p->offset*4);
	ptr = (unsigned char *)p + 20;
	while(ptr < optend)
	{
		if(*ptr==0)
			return;
//		if(*ptr==1) do nothing
		if(*ptr==2)
		{
			ptr++;
			if(*ptr==4)
			{
				ptr++;
				this->tx_mss = *ptr;
				ptr++;
 				this->tx_mss += (*ptr)<<8;
			}
		}	
		ptr++;
	}
}
