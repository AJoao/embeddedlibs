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

#ifndef _IP_H
#define _IP_H

#define IPPROTO_IP 		0
#define IPPROTO_HOPOPTS	 	0
#define IPPROTO_ICMP		1
#define IPPROTO_IGMP		2
#define IPPROTO_IPIP		4
#define IPPROTO_TCP		6
#define IPPROTO_EGP		8
#define IPPROTO_PUP		12
#define IPPROTO_UDP		17
#define IPPROTO_IDP		22
#define IPPROTO_TP		29
#define IPPROTO_DCCP		33
#define IPPROTO_IPV6		41
#define IPPROTO_ROUTING		43
#define IPPROTO_FRAGMENT	44
#define IPPROTO_RSVP		46
#define IPPROTO_GRE		47
#define IPPROTO_ESP		50
#define IPPROTO_AH		51
#define IPPROTO_ICMPV6		58
#define IPPROTO_NONE		59
#define IPPROTO_DSTOPTS		60
#define IPPROTO_MTP		92
#define IPPROTO_ENCAP		98
#define IPPROTO_PIM		103
#define IPPROTO_COMP		108
#define IPPROTO_SCTP		132
#define IPPROTO_UDPLITE		136
#define IPPROTO_RAW		255
/*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
struct ip_packet
{
	unsigned ihl:4;
	unsigned version:4;
	unsigned char tos;
	unsigned short tot_len;
	unsigned short id;
	unsigned frag_offh:5;
	unsigned flags:3;
	unsigned char frag_off;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short check;
	unsigned char saddr[4];
	unsigned char daddr[4];
} __attribute__((packed));

#endif
