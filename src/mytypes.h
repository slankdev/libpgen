#ifndef TYPES_H
#define TYPES_H

#include "pgen-variable.h"






struct myEthHdr
{
//  u_int8_t  ether_dhost[ETH_ALEN];	/* destination eth addr	*/
//  u_int8_t  ether_shost[ETH_ALEN];	/* source ether addr	*/
//  u_int16_t ether_type;		        /* packet type ID field	*/
};



struct myArpHdr
{
//    unsigned short int ar_hrd;		/* Format of hardware address.  */
//    unsigned short int ar_pro;		/* Format of protocol address.  */
//    unsigned char ar_hln;		/* Length of hardware address.  */
//    unsigned char ar_pln;		/* Length of protocol address.  */
//    unsigned short int ar_op;		/* ARP opcode (command).  */
};

#define	ARPOP_REQUEST	1		/* ARP request.  */
#define	ARPOP_REPLY	2		/* ARP reply.  */
#define	ARPOP_RREQUEST	3		/* RARP request.  */
#define	ARPOP_RREPLY	4		/* RARP reply.  */
#define	ARPOP_InREQUEST	8		/* InARP request.  */
#define	ARPOP_InREPLY	9		/* InARP reply.  */
#define	ARPOP_NAK	10		/* (ATM)ARP NAK.  */

struct	myArp {
//	struct	arphdr ea_hdr;		/* fixed-size header */
//	u_int8_t arp_sha[ETH_ALEN];	/* sender hardware address */
//	u_int8_t arp_spa[4];		/* sender protocol address */
//	u_int8_t arp_tha[ETH_ALEN];	/* target hardware address */
//	u_int8_t arp_tpa[4];		/* target protocol address */
};



/*
struct myIpHdr
{
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
};*/



#endif
