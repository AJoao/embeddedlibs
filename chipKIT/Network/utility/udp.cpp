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

UDP::UDP(unsigned short port)
{
	this->port = port;
	this->rx_head = 0;
	this->rx_tail = 0;
}

void UDP::addPacket(unsigned short len, struct ethernet_packet *packet)
{
	memcpy((void *)&(this->rx[this->rx_head].data),(void *)packet,len);
	this->rx[this->rx_head].len = len;
	this->rx_head++;
	if(this->rx_head == ETH_BUF_SIZE)
		this->rx_head = 0;
}

unsigned short UDP::available()
{
	if(this->rx_head == this->rx_tail)
		return 0;
	if(this->rx_head > this->rx_tail)
		return this->rx_head - this->rx_tail;
	return (ETH_BUF_SIZE-this->rx_tail) + this->rx_head;
}

unsigned short UDP::recv(unsigned char *buf)
{
	struct ip_packet *ip;
	unsigned short ipdoff;
	unsigned short ipdlen;
	unsigned char *ipdata;
	unsigned short len;
	struct ethernet_packet *rx;
	struct udp_packet *udp;
	if(this->available()==0)
		return 0;

	rx = &(this->rx[this->rx_tail].data);
	ip = (struct ip_packet *)(rx->payload);
	ipdoff = ip->ihl << 2;
	ipdlen = ntohs(ip->tot_len) - ipdoff;
	ipdata = (unsigned char *)(rx->payload)+ipdoff;
	udp = (struct udp_packet *)ipdata;

	len = ntohs(udp->length);

	memcpy(buf,(char *)udp+sizeof(struct udp_packet),len);
	this->rx_tail++;
	if(this->rx_tail == ETH_BUF_SIZE)
		this->rx_tail = 0;

	return len;
}

unsigned long UDP::peerAddress()
{
	struct ip_packet *ip;
	unsigned short ipdoff;
	unsigned short ipdlen;
	unsigned char *ipdata;
	unsigned short len;
	unsigned long peer;
	struct ethernet_packet *rx;
	struct udp_packet *udp;
	if(this->available()==0)
		return 0;

	rx = &(this->rx[this->rx_tail].data);
	ip = (struct ip_packet *)(rx->payload);
	ipdoff = ip->ihl << 2;
	ipdlen = ntohs(ip->tot_len) - ipdoff;
	ipdata = (unsigned char *)(rx->payload)+ipdoff;
	udp = (struct udp_packet *)ipdata;
	peer = ip->saddr[0]<<24 | ip->saddr[1]<<16 | ip->saddr[2]<<8 | ip->saddr[3];
	return peer;
}

unsigned short UDP::peerPort()
{
	struct ip_packet *ip;
	unsigned short ipdoff;
	unsigned short ipdlen;
	unsigned char *ipdata;
	unsigned short len;
	struct ethernet_packet *rx;
	struct udp_packet *udp;
	if(this->available()==0)
		return 0;

	rx = &(this->rx[this->rx_tail].data);
	ip = (struct ip_packet *)(rx->payload);
	ipdoff = ip->ihl << 2;
	ipdlen = ntohs(ip->tot_len) - ipdoff;
	ipdata = (unsigned char *)(rx->payload)+ipdoff;
	udp = (struct udp_packet *)ipdata;
	return ntohs(udp->sourcePort);
}

short UDP::send(unsigned long peerAddress, unsigned short peerPort, unsigned char *data, unsigned short len)
{
	short interface;
	struct ethernet_packet eth;
	unsigned long ipAddress = peerAddress;
	short rv;
	struct ip_packet *ip; 
	unsigned char *ipdata;
	struct udp_packet *udp;
	unsigned char *udpdata;

	interface = Network.getInterfaceByIP(peerAddress);
	if(interface<0)
	{
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
	ip->protocol = IPPROTO_UDP;
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
	ip->daddr[3] = ipAddress & 0xFF;
	ip->daddr[2] = ipAddress>>8 & 0xFF;
	ip->daddr[1] = ipAddress>>16 & 0xFF;
	ip->daddr[0] = ipAddress>>24 & 0xFF;
	ip->version = 4;
	ip->ihl = 5;
	ip->tot_len = htons(sizeof(struct udp_packet) + 20 + len);
	ip->check = 0;
	ipdata = (unsigned char *)ip + 20;
	udp = (struct udp_packet *)ipdata;
	udp->sourcePort = htons(this->port);
	udp->destPort = htons(peerPort);
	udp->checksum = 0;
	udp->length = htons(len+sizeof(udp_packet));
	udpdata = (unsigned char *)udp+sizeof(struct udp_packet);
	memcpy(udpdata,data,len);
	udp->checksum = htons(checksum16((unsigned char *)&ip->saddr[0],len+16,1));
	ip->check = htons(checksum16((unsigned char *)ip,20,CS_IP));


	Network.interfaces[interface]->queueTX((unsigned char *)&eth,sizeof(struct ether_header)+20+sizeof(struct udp_packet)+len);
	return len;
}
