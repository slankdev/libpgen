

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#ifndef PGEN_TYPES_H
#define PGEN_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include "pgen-endian.h"		// for BYTE_ORDER



typedef struct pgen pgen_t;
typedef unsigned char      bit8;
typedef unsigned short     bit16;
typedef unsigned int       bit32;
typedef unsigned long      bit64;
typedef unsigned long long bit128;

struct pcap_timeval{
	bit32 tv_sec;
	bit32 tv_usec;
};



struct pcap_fhdr{
	bit32 magic;
	bit16 version_major;
	bit16 version_minor;
	bit32 timezone;
	bit32 sigfigs;
	bit32 snaplen;
	bit32 linktype;
};



struct pcap_pkthdr{
	struct pcap_timeval ts;
	bit32 caplen;
	bit32 len;
};




#define PGEN_PCAPFILE_MAXLEN 100000000
struct pgen_opt{
	int is_timeout;
	struct timeval timeout;
};


struct pgen{
	int fd;
	int is_offline;
	int is_write;
	int is_read;
	struct{
		FILE* fd;
		struct pcap_fhdr filehdr;
	}offline;
	struct pgen_opt opt;
};






#define ETH_HDR_LEN  sizeof(struct ethernet_header)
#define ARP_HDR_LEN  sizeof(struct arp_packet)
#define IP_HDR_LEN   sizeof(struct ip_header)
#define ICMP_HDR_LEN sizeof(struct icmp_header)
#define TCP_HDR_LEN  sizeof(struct tcp_header)
#define UDP_HDR_LEN  sizeof(struct udp_header)
#define DNS_HDR_LEN  sizeof(struct dns_header)
#define DHCP_HDR_LEN sizeof(struct dhcp_header)



struct ethernet_header{
	bit8  ether_dhost[6];	/* destination eth addr	*/
	bit8  ether_shost[6];	/* source ether addr	*/
	bit16 ether_type;		        /* packet type ID field	*/
};






struct arp_header{
	bit16 ar_hrd;		/* Format of hardware address.  */
	bit16 ar_pro;		/* Format of protocol address.  */
	bit8  ar_hln;			/* Length of hardware address.  */
	bit8  ar_pln;			/* Length of protocol address.  */
	bit16 ar_op;		/* ARP opcode (command).  */
};

struct	arp_packet{
	struct arp_header  ea_hdr;	/* fixed-size header */
	bit8 arp_sha[6];		/* sender hardware address */
	bit8 arp_spa[4];		/* sender protocol address */
	bit8 arp_tha[6];		/* target hardware address */
	bit8 arp_tpa[4];		/* target protocol address */
};







struct ip_header{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	bit32 ihl:4;
	bit32 version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	bit32 version:4;
	bit32 ihl:4;
#else
# error	"Please fix endian.h"
#endif
	bit8  tos;
	bit16 tot_len;
	bit16 id;
	bit16 frag_off;
	bit8  ttl;
	bit8  protocol;
	bit16 check;
	bit32 saddr;
	bit32 daddr;
	/*The options start here. */
};


struct icmp_echo_header{
	bit16 id;
	bit16 seq;
};

struct icmp_destination_unreach{
	bit8  nouse;
	bit8  len;
	bit16 next_mtu;
};

struct icmp_redirect{
	bit32 gw_addr;
};

struct icmp_time_exceeded{
	bit8  nouse1;
	bit8  len;
	bit16 nouse2;
};


struct icmp_header{
	bit8  icmp_type;
	bit8  icmp_code;
	bit16 icmp_cksum;
};



struct tcp_header{
	bit16 source;
	bit16 dest;
	bit32 seq;
	bit32 ack_seq;
# if __BYTE_ORDER == __LITTLE_ENDIAN
	bit16 res1:4;
	bit16 doff:4;
	bit16 fin:1;
	bit16 syn:1;
	bit16 rst:1;
	bit16 psh:1;
	bit16 ack:1;
	bit16 urg:1;
	bit16 res2:2;
# elif __BYTE_ORDER == __BIG_ENDIAN
	bit16 doff:4;
	bit16 res1:4;
	bit16 res2:2;
	bit16 urg:1;
	bit16 ack:1;
	bit16 psh:1;
	bit16 rst:1;
	bit16 syn:1;
	bit16 fin:1;
# else
#  error "Please fix endian.h"
# endif
	bit16 window;
	bit16 check;
	bit16 urg_ptr;
};




struct udp_header{
	bit16 source;
	bit16 dest;
	bit16 len;
	bit16 check;
}; 





struct dns_header{
	bit16 id;
	union{
		struct{
# if __BYTE_ORDER == __LITTLE_ENDIAN
			bit16 rd:1;
			bit16 tc:1;
			bit16 aa:1;
			bit16 opcode:4;
			bit16 qr:1;
			bit16 rcode:4;
			bit16 nouse:3;
			bit16 ra:1;
# elif __BYTE_ORDER == __BIG_ENDIAN	
			bit16 qr:1;
			bit16 opcode:4;
			bit16 aa:1;
			bit16 tc:1;
			bit16 rd:1;
			bit16 ra:1;
			bit16 nouse:3;
			bit16 rcode:4;
# else 
# 	error "Please fix endian.h"
# endif
		};
		bit16 flags;
	};
	bit16 qdcnt;
	bit16 ancnt;
	bit16 nscnt;
	bit16 arcnt;
};




struct dhcp_option{
	bit8 type;
	bit8 len;
	bit8 data[128];
};



struct dhcp_header{
	bit8   op;
	bit8   htype;
	bit8   hlen;
	bit8   hops;
	bit32  xid;
	bit16  secs;
	bit16  flags;
	bit32  ciaddr;
	bit32  yiaddr;
	bit32  siaddr;
	bit32  giaddr;
	bit8   chaddr[16];
	bit8   sname[64];
	bit8   file[128];
	bit8   magic[4];
};





#endif /* PGEN_TYPES_H */
