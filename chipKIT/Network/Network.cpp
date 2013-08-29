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

_Network Network;

void network_tick()
{
	Network.tick();
}

//void __attribute__((vector(_T4_VECTOR),interrupt(IPL4))) T4Interrupt(void)
extern "C" {
        void __ISR(_TIMER_4_VECTOR, _T4_IPL_ISR) T4_IntHandler (void)
        {
                network_tick();
                IFS0bits.T4IF=0;
        }
}

_Network::_Network()
{
	unsigned short i;
	for(i=0; i<MAX_INTERFACES; i++)
	{
		this->interfaces[i] = NULL;
	}
        T4CON = 0x0050; // set prescalar 1:32
        TMR4 = 0;
        PR4 = 0x61A8;
        IFS0CLR = 0x10000;// Clear the T4 interrupt flag
        IEC0SET = 0x10000;// Enable T4 interrupt

        IPC4CLR = 0x0000001F;
        IPC4SET = (_T4_IPL_IPC << 2) | _T4_SPL_IPC;
        T4CONSET = 0x8000;// Enable Timer4

	pinMode(PIN_LED1,OUTPUT);
	pinMode(PIN_LED2,OUTPUT);

	digitalWrite(PIN_LED1,LOW);
	digitalWrite(PIN_LED2,LOW);
}

void _Network::tick()
{
	static unsigned short tx_to=0;
	static unsigned short rx_to=0;
	struct ethernet_packet rx;
	unsigned short len;
	unsigned short i;
	ENC28J60 *iface;
	struct arp_packet *arp;
	struct ip_packet *ip;
	struct icmp_packet *icmp;
	struct udp_packet *udp;
	struct tcp_packet *tcp;
	char temp[100];
	unsigned long spa,tpa;
	unsigned char *ipdata;
	unsigned short ipdlen;
	unsigned short ipdoff;
	TCPClient *cli;
	TCPServer *svr;
	TCPClient *est;

	if(rx_to>0)
	{
		rx_to--;
		if(rx_to==0)
		{
			digitalWrite(PIN_LED1,LOW);
		}
	}

	if(tx_to>0)
	{
		tx_to--;
		if(tx_to==0)
		{
			digitalWrite(PIN_LED2,LOW);
		}
	}

	for(i=0; i<MAX_INTERFACES; i++)
	{
		if(this->interfaces[i]!=NULL)
		{
			iface = this->interfaces[i];
			len = iface->packetReceive(1500,(unsigned char *)&rx);
			if(len>0)
			{
				iface->rx_packets++;
				// Handle the incoming packet
				digitalWrite(PIN_LED1,HIGH);
				rx_to = 10;
				switch(ntohs(rx.header.ether_type))
				{
					case ETHERTYPE_ARP:
						arp = (struct arp_packet *)rx.payload;
						spa = arp->spa[0]<<24 | arp->spa[1]<<16 | arp->spa[2]<<8 | arp->spa[3];
						tpa = arp->tpa[0]<<24 | arp->tpa[1]<<16 | arp->tpa[2]<<8 | arp->tpa[3];
						if(tpa == iface->ip)
						{
							this->addOrUpdateARP(arp->sha,spa,i);
							if(ntohs(arp->op) == ARP_OP_REQUEST)
							{
								// Someone is looking for us.
								memcpy(arp->tha,arp->sha,6);
								memcpy(arp->tpa,arp->spa,4);
								memcpy(arp->sha,iface->mac,6);
								arp->spa[3] = iface->ip & 0xFF;
								arp->spa[2] = iface->ip>>8 & 0xFF;
								arp->spa[1] = iface->ip>>16 & 0xFF;
								arp->spa[0] = iface->ip>>24 & 0xFF;
								memcpy(rx.header.ether_dhost,rx.header.ether_shost,6);
								memcpy(rx.header.ether_shost,iface->mac,6);
								arp->op = htons(ARP_OP_REPLY);

								iface->queueTX((unsigned char *)&rx,len);
							}
						}
						break;
					case ETHERTYPE_IP:
						ip = (struct ip_packet *)rx.payload;

						ipdoff = ip->ihl << 2;
						ipdlen = ntohs(ip->tot_len) - ipdoff;
						ipdata = (unsigned char *)(&rx.payload)+ipdoff;
						switch(ip->protocol)
						{
							case IPPROTO_ICMP:
								icmp = (struct icmp_packet *)ipdata;

								switch(icmp->type)
								{
									case ICMP_ECHO:
										icmp->type = ICMP_ECHOREPLY;
										memcpy(rx.header.ether_dhost,rx.header.ether_shost,6);
										memcpy(rx.header.ether_shost,iface->mac,6);
										memcpy(ip->daddr,ip->saddr,4);
										ip->saddr[3] = iface->ip & 0xFF;
										ip->saddr[2] = iface->ip>>8 & 0xFF;
										ip->saddr[1] = iface->ip>>16 & 0xFF;
										ip->saddr[0] = iface->ip>>24 & 0xFF;
										
										icmp->checksum = 0;
										icmp->checksum = htons(checksum16(ipdata,ipdlen,CS_IP));

										iface->queueTX((unsigned char *)&rx,len);
										break;
								}
								break;
							case IPPROTO_UDP:
								udp = (struct udp_packet *)ipdata;
								for(i=0; i<MAX_PORTS; i++)
								{
									if(this->udp_ports[i] != NULL)
									{
										if(this->udp_ports[i]->port == ntohs(udp->destPort))
										{
											this->udp_ports[i]->addPacket(len,&rx);
										}
									}
								}
								break;
							case IPPROTO_TCP:
								tcp = (struct tcp_packet *)ipdata;
								for(i=0; i<MAX_PORTS; i++)
								{
									if(this->tcp_clients[i]!=NULL)
									{
										if(this->tcp_clients[i]->socketMatch(ntohs(tcp->destPort), ntohs(tcp->sourcePort), 
											quad2ip(ip->saddr[0],ip->saddr[1],ip->saddr[2],ip->saddr[3]))==1)
										{
											cli = this->tcp_clients[i];
											cli->handleIncomingPacket(ip);
										}
									}
									if(this->tcp_servers[i]!=NULL)
									{
										// Packet coming in to a listening socket
										if((this->tcp_servers[i]->localPort == ntohs(tcp->destPort)) &&
											(this->tcp_servers[i]->state == TCP_LISTEN))
										{
											svr = this->tcp_servers[i];
											// Try and match it with an existing connection
											cli = NULL;
											for(i=0; i<MAX_TCP_CONNECTIONS; i++)
											{
												if(svr->connections[i].state!=TCP_CLOSED)
												{
													if(svr->connections[i].socketMatch(ntohs(tcp->destPort), ntohs(tcp->sourcePort), 
														quad2ip(ip->saddr[0],ip->saddr[1],ip->saddr[2],ip->saddr[3]))==1)
													{
														cli = &svr->connections[i];
														cli->handleIncomingPacket(ip);
													}
												}
											}
											// Not an existing connections.  If it's a SYN then start a connection up
											if(cli==NULL)
											{
												if(tcp->syn==1 && tcp->ack==0)
												{
													est = svr->getFreeConnection();
													if(est!=NULL)
													{
														est->localPort = svr->localPort;
														est->peerPort = ntohs(tcp->sourcePort);
														est->peerAddress = quad2ip(ip->saddr[0],ip->saddr[1],ip->saddr[2],ip->saddr[3]);
														est->tx_mss=1440;
														est->irs = ntohl(tcp->sequence);
														est->rx_nxt = est->irs+1;
														est->iss = rand();
														est->tx_nxt = est->iss;
														est->sendSynAck();
														est->state=TCP_ESTABLISHED;
													}
												}
											}
										}
									}
								}
								break;
							default:
								break;
						}
						break;
					default:
						break;
				}
			}
			while(iface->tx_head != iface->tx_tail)
			{
				iface->tx_packets++;
				digitalWrite(PIN_LED2,HIGH);
				tx_to = 10;
				iface->packetSend(iface->tx[iface->tx_tail].len, (unsigned char *)&(iface->tx[iface->tx_tail].data));
				iface->tx_tail++;
				if(iface->tx_tail==ETH_BUF_SIZE)
					iface->tx_tail = 0;
			}
		}
	}
}

short _Network::addInterface(ENC28J60 *card)
{
	unsigned short i;
	for(i=0; i<MAX_INTERFACES; i++)
	{
		if(this->interfaces[i] == NULL)
		{
			this->interfaces[i] = card;
			this->interfaces[i]->tx_packets = 0;
			this->interfaces[i]->rx_packets = 0;
			return i;
		}
	}
	return -1;
}

short _Network::getARP(unsigned long ip, unsigned char *mac, short iface)
{
	struct ethernet_packet eth;
	unsigned long to;
	struct arp_packet *arp;
	unsigned short i;
	for(i=0; i<MAX_ARP; i++)
	{
		if(this->arp[i].ip == ip)
		{
			memcpy(mac,this->arp[i].mac,6);
			return 1;
		}
	}

	// No existing entry found - let's try requesting one.
	eth.header.ether_type = htons(ETHERTYPE_ARP);
	memcpy(eth.header.ether_shost,this->interfaces[iface]->mac,6);
	memcpy(eth.header.ether_dhost,"\xff\xff\xff\xff\xff\xff",6);
	arp = (struct arp_packet *)&eth.payload;
	memcpy(arp->sha,this->interfaces[iface]->mac,6);
	memcpy(arp->tha,"\0\0\0\0\0\0",6);
	arp->tpa[3] = ip & 0xFF;
	arp->tpa[2] = ip>>8 & 0xFF;
	arp->tpa[1] = ip>>16 & 0xFF;
	arp->tpa[0] = ip>>24 & 0xFF;
	arp->spa[3] = this->interfaces[iface]->ip & 0xFF;
	arp->spa[2] = this->interfaces[iface]->ip>>8 & 0xFF;
	arp->spa[1] = this->interfaces[iface]->ip>>16 & 0xFF;
	arp->spa[0] = this->interfaces[iface]->ip>>24 & 0xFF;
	arp->hrd = htons(1);
	arp->pro = htons(0x0800);
	arp->hln = 6;
	arp->pln = 4;
	arp->op = htons(ARP_OP_REQUEST);
	this->interfaces[iface]->queueTX((unsigned char *)&eth,sizeof(struct ether_header)+sizeof(struct arp_packet));

	to = millis();
	while(millis() - to < 1000)
	{
		for(i=0; i<MAX_ARP; i++)
		{
			if(this->arp[i].ip == ip)
			{
				memcpy(mac,this->arp[i].mac,6);
				return 1;
			}
		}
	}
	return -1;
}

void _Network::addOrUpdateARP(unsigned char mac[6], unsigned long ip, unsigned short iface)
{
	unsigned short i;
	unsigned long mints;
	unsigned short mini;

	// First look to update an existing entry.
	for(i=0; i<MAX_ARP; i++)
	{
		if(this->arp[i].ip == ip)
		{
			memcpy(this->arp[i].mac,mac,6);
			this->arp[i].iface = iface;
			this->arp[i].ts = millis();
			return;
		}
	}

	// Not found one?  Ok, let's look for room to store a new one.
	for(i=0; i<MAX_ARP; i++)
	{
		if(this->arp[i].ts==0)
		{
			this->arp[i].ip = ip;
			memcpy(this->arp[i].mac,mac,6);
			this->arp[i].iface = iface;
			this->arp[i].ts = millis();
			return;
		}
	}

	// No room?  Ok, let's find the oldest one and take it over.
	mints = millis();
	mini = 0xFFFF;

	for(i=0; i<MAX_ARP; i++)
	{
		if(this->arp[i].ts < mints)
		{
			mints = this->arp[i].ts;
			mini = i;
		}
	}

	if(mini == 0xFFFF)
	{
		// Oops - something went wrong.
		return;
	}

	this->arp[mini].ip = ip;
	memcpy(this->arp[mini].mac,mac,6);
	this->arp[mini].iface = iface;
	this->arp[mini].ts = millis();
}

short _Network::addPort(UDP *port)
{
	unsigned short i;
	for(i=0; i<MAX_PORTS; i++)
	{
		if(this->udp_ports[i] == NULL)
		{
			this->udp_ports[i] = port;
			return i;
		}
	}
	return -1;
}

short _Network::addPort(TCPClient *port)
{
	unsigned short i;
	for(i=0; i<MAX_PORTS; i++)
	{
		if(this->tcp_clients[i] == NULL)
		{
			this->tcp_clients[i] = port;
			return i;
		}
	}
	return -1;
}

short _Network::addPort(TCPServer *port)
{
	unsigned short i;
	for(i=0; i<MAX_PORTS; i++)
	{
		if(this->tcp_servers[i] == NULL)
		{
			this->tcp_servers[i] = port;
			return i;
		}
	}
	return -1;
}

short _Network::getInterfaceByIP(unsigned long ip)
{
	unsigned short i;
	unsigned long mask;
	unsigned long maskedIP;
	unsigned long myMaskedIP;
	for(i=0; i<MAX_INTERFACES; i++)
	{
		if(this->interfaces[i]!=NULL)
		{
			mask = this->interfaces[i]->netmask;
			myMaskedIP = this->interfaces[i]->ip & mask;
			maskedIP = ip & mask;
			if(maskedIP == myMaskedIP)
			{
				return i;
			}
		}
	}
	return -1;
}

void _Network::setDefaultRouter(unsigned long ip)
{
	this->defaultRouter = ip;
}

short _Network::getDefaultInterface()
{
	if(!this->defaultRouter)
		return -1;
	return this->getInterfaceByIP(this->defaultRouter);
}

