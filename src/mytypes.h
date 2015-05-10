#ifndef TYPES_H
#define TYPES_H

#include "pgen-variable.h"






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
