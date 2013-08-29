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

#ifndef _ICMP_H
#define _ICMP_H

struct icmp_packet
{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	union
	{
		struct
		{
			unsigned short id;
			unsigned short sequence;
		} echo;
		unsigned long   gateway;
		struct
		{
			unsigned short __unused;
			unsigned short mtu;
		} frag;
	} un __attribute__((packed));
};

#define ICMP_ECHOREPLY          0       /* Echo Reply                   */
#define ICMP_DEST_UNREACH       3       /* Destination Unreachable      */
#define ICMP_SOURCE_QUENCH      4       /* Source Quench                */
#define ICMP_REDIRECT           5       /* Redirect (change route)      */
#define ICMP_ECHO               8       /* Echo Request                 */
#define ICMP_TIME_EXCEEDED      11      /* Time Exceeded                */
#define ICMP_PARAMETERPROB      12      /* Parameter Problem            */
#define ICMP_TIMESTAMP          13      /* Timestamp Request            */
#define ICMP_TIMESTAMPREPLY     14      /* Timestamp Reply              */
#define ICMP_INFO_REQUEST       15      /* Information Request          */
#define ICMP_INFO_REPLY         16      /* Information Reply            */
#define ICMP_ADDRESS            17      /* Address Mask Request         */
#define ICMP_ADDRESSREPLY       18      /* Address Mask Reply           */
#define NR_ICMP_TYPES           18


/* Codes for UNREACH. */
#define ICMP_NET_UNREACH        0       /* Network Unreachable          */
#define ICMP_HOST_UNREACH       1       /* Host Unreachable             */
#define ICMP_PROT_UNREACH       2       /* Protocol Unreachable         */
#define ICMP_PORT_UNREACH       3       /* Port Unreachable             */
#define ICMP_FRAG_NEEDED        4       /* Fragmentation Needed/DF set  */
#define ICMP_SR_FAILED          5       /* Source Route failed          */
#define ICMP_NET_UNKNOWN        6
#define ICMP_HOST_UNKNOWN       7
#define ICMP_HOST_ISOLATED      8
#define ICMP_NET_ANO            9
#define ICMP_HOST_ANO           10
#define ICMP_NET_UNR_TOS        11
#define ICMP_HOST_UNR_TOS       12
#define ICMP_PKT_FILTERED       13      /* Packet filtered */
#define ICMP_PREC_VIOLATION     14      /* Precedence violation */
#define ICMP_PREC_CUTOFF        15      /* Precedence cut off */
#define NR_ICMP_UNREACH         15      /* instead of hardcoding immediate value */

/* Codes for REDIRECT. */
#define ICMP_REDIR_NET          0       /* Redirect Net                 */
#define ICMP_REDIR_HOST         1       /* Redirect Host                */
#define ICMP_REDIR_NETTOS       2       /* Redirect Net for TOS         */
#define ICMP_REDIR_HOSTTOS      3       /* Redirect Host for TOS        */

/* Codes for TIME_EXCEEDED. */
#define ICMP_EXC_TTL            0       /* TTL count exceeded           */
#define ICMP_EXC_FRAGTIME       1       /* Fragment Reass time exceeded */



#endif
