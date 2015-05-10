#ifndef TYPES_H
#define TYPES_H

#include "pgen-variable.h"

#include <netinet/in.h> // for BYTE_ORDER
#include <endian.h>		// for BYTE_ORDER





struct MYETH{
	u_int8_t  ether_dhost[6];	/* destination eth addr	*/
	u_int8_t  ether_shost[6];	/* source ether addr	*/
	u_int16_t ether_type;		        /* packet type ID field	*/
};








struct myArpHdr{
	unsigned short int ar_hrd;		/* Format of hardware address.  */
	unsigned short int ar_pro;		/* Format of protocol address.  */
	unsigned char ar_hln;		/* Length of hardware address.  */
	unsigned char ar_pln;		/* Length of protocol address.  */
	unsigned short int ar_op;		/* ARP opcode (command).  */
};

#define	ARPOP_REQUEST	1		/* ARP request.  */
#define	ARPOP_REPLY	2		/* ARP reply.  */
#define	ARPOP_RREQUEST	3		/* RARP request.  */
#define	ARPOP_RREPLY	4		/* RARP reply.  */
#define	ARPOP_InREQUEST	8		/* InARP request.  */
#define	ARPOP_InREPLY	9		/* InARP reply.  */
#define	ARPOP_NAK	10		/* (ATM)ARP NAK.  */

struct	MYARP{
	struct myArpHdr  ea_hdr;		/* fixed-size header */
	u_int8_t arp_sha[6];	/* sender hardware address */
	u_int8_t arp_spa[4];		/* sender protocol address */
	u_int8_t arp_tha[6];	/* target hardware address */
	u_int8_t arp_tpa[4];		/* target protocol address */
};








struct MYIP{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int ihl:4;
	unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int version:4;
	unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
	u_int8_t tos;
	u_int16_t tot_len;
	u_int16_t id;
	u_int16_t frag_off;
	u_int8_t ttl;
	u_int8_t protocol;
	u_int16_t check;
	u_int32_t saddr;
	u_int32_t daddr;
	/*The options start here. */
};









typedef	u_int32_t tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct MYTCP{
	__extension__ union{
		struct{
			u_int16_t th_sport;		/* source port */
			u_int16_t th_dport;		/* destination port */
			tcp_seq th_seq;		/* sequence number */
			tcp_seq th_ack;		/* acknowledgement number */
# if __BYTE_ORDER == __LITTLE_ENDIAN
			u_int8_t th_x2:4;		/* (unused) */
			u_int8_t th_off:4;		/* data offset */
# endif
# if __BYTE_ORDER == __BIG_ENDIAN
			u_int8_t th_off:4;		/* data offset */
			u_int8_t th_x2:4;		/* (unused) */
# endif
			u_int8_t th_flags;
# define TH_FIN	0x01
# define TH_SYN	0x02
# define TH_RST	0x04
# define TH_PUSH	0x08
# define TH_ACK	0x10
# define TH_URG	0x20
			u_int16_t th_win;		/* window */
			u_int16_t th_sum;		/* checksum */
			u_int16_t th_urp;		/* urgent pointer */
		};
		struct{
			u_int16_t source;
			u_int16_t dest;
			u_int32_t seq;
			u_int32_t ack_seq;
# if __BYTE_ORDER == __LITTLE_ENDIAN
			u_int16_t res1:4;
			u_int16_t doff:4;
			u_int16_t fin:1;
			u_int16_t syn:1;
			u_int16_t rst:1;
			u_int16_t psh:1;
			u_int16_t ack:1;
			u_int16_t urg:1;
			u_int16_t res2:2;
# elif __BYTE_ORDER == __BIG_ENDIAN
			u_int16_t doff:4;
			u_int16_t res1:4;
			u_int16_t res2:2;
			u_int16_t urg:1;
			u_int16_t ack:1;
			u_int16_t psh:1;
			u_int16_t rst:1;
			u_int16_t syn:1;
			u_int16_t fin:1;
# else
#  error "Adjust your <bits/endian.h> defines"
# endif
			u_int16_t window;
			u_int16_t check;
			u_int16_t urg_ptr;
		};
	};
};









struct MYICMP{
	u_int8_t  icmp_type;	/* type of message, see below */
	u_int8_t  icmp_code;	/* type sub code */
	u_int16_t icmp_cksum;	/* ones complement checksum of struct */
	union{
		u_char ih_pptr;		/* ICMP_PARAMPROB */
		struct in_addr ih_gwaddr;	/* gateway address */
		struct ih_idseq{		/* echo datagram */
			u_int16_t icd_id;
			u_int16_t icd_seq;
		} ih_idseq;
		u_int32_t ih_void;

		/* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
		struct ih_pmtu{
			u_int16_t ipm_void;
			u_int16_t ipm_nextmtu;
		} ih_pmtu;

		struct ih_rtradv{
			u_int8_t irt_num_addrs;
			u_int8_t irt_wpa;
			u_int16_t irt_lifetime;
		} ih_rtradv;
	} icmp_hun;
#define	icmp_pptr	icmp_hun.ih_pptr
#define	icmp_gwaddr	icmp_hun.ih_gwaddr
#define	icmp_id		icmp_hun.ih_idseq.icd_id
#define	icmp_seq	icmp_hun.ih_idseq.icd_seq
#define	icmp_void	icmp_hun.ih_void
#define	icmp_pmvoid	icmp_hun.ih_pmtu.ipm_void
#define	icmp_nextmtu	icmp_hun.ih_pmtu.ipm_nextmtu
#define	icmp_num_addrs	icmp_hun.ih_rtradv.irt_num_addrs
#define	icmp_wpa	icmp_hun.ih_rtradv.irt_wpa
#define	icmp_lifetime	icmp_hun.ih_rtradv.irt_lifetime
	union{
		struct{
			u_int32_t its_otime;
			u_int32_t its_rtime;
			u_int32_t its_ttime;
		} id_ts;
		struct{
			struct ip idi_ip;
			/* options and then 64 bits of data */
		} id_ip;
		struct icmp_ra_addr id_radv;
		u_int32_t   id_mask;
		u_int8_t    id_data[1];
	} icmp_dun;
#define	icmp_otime	icmp_dun.id_ts.its_otime
#define	icmp_rtime	icmp_dun.id_ts.its_rtime
#define	icmp_ttime	icmp_dun.id_ts.its_ttime
#define	icmp_ip		icmp_dun.id_ip.idi_ip
#define	icmp_radv	icmp_dun.id_radv
#define	icmp_mask	icmp_dun.id_mask
#define	icmp_data	icmp_dun.id_data
};










struct MYUDP{
	__extension__ union{
		struct{
			u_int16_t uh_sport;		/* source port */
			u_int16_t uh_dport;		/* destination port */
			u_int16_t uh_ulen;		/* udp length */
			u_int16_t uh_sum;		/* udp checksum */
		};
		struct{
			u_int16_t source;
			u_int16_t dest;
			u_int16_t len;
			u_int16_t check;
		};
	};
};






#endif
