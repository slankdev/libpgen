#ifndef TYPES_H
#define TYPES_H


#include <netinet/in.h> // for BYTE_ORDER
#include <endian.h>		// for BYTE_ORDER

#include <netinet/ip.h>				// i want delete this
#include <netinet/ip_icmp.h>		// i want delete this

typedef u_int8_t  bit8;
typedef u_int16_t bit16;
typedef u_int32_t bit32;
typedef uint64_t  bit64;
typedef unsigned long long bit128;



 
#define	MT_ETHERTYPE_PUP		0x0200		/* Xerox PUP */
#define MT_ETHERTYPE_SPRITE		0x0500		/* Sprite */
#define	MT_ETHERTYPE_IP			0x0800		/* IP */
#define	MT_ETHERTYPE_ARP		0x0806		/* Address resolution */
#define	MT_ETHERTYPE_REVARP		0x8035		/* Reverse ARP */
#define MT_ETHERTYPE_AT			0x809B		/* AppleTalk protocol */
#define MT_ETHERTYPE_AARP		0x80F3		/* AppleTalk ARP */
#define	MT_ETHERTYPE_VLAN		0x8100		/* IEEE 802.1Q VLAN tagging */
#define MT_ETHERTYPE_IPX		0x8137		/* IPX */
#define	MT_ETHERTYPE_IPV6		0x86dd		/* IP protocol version 6 */
#define MT_ETHERTYPE_LOOPBACK	0x9000		/* used to test interfaces */

struct MYETH{
	bit8  ether_dhost[6];	/* destination eth addr	*/
	bit8  ether_shost[6];	/* source ether addr	*/
	bit16 ether_type;		        /* packet type ID field	*/
};





/* ARP protocol HARDWARE identifiers. */
#define MT_ARPHRD_NETROM		0		/* From KA9Q: NET/ROM pseudo. */
#define MT_ARPHRD_ETHER			1		/* Ethernet 10/100Mbps.  */
#define	MT_ARPHRD_EETHER		2		/* Experimental Ethernet.  */
#define	MT_ARPHRD_AX25			3		/* AX.25 Level 2.  */
#define	MT_ARPHRD_PRONET		4		/* PROnet token ring.  */
#define	MT_ARPHRD_CHAOS			5		/* Chaosnet.  */
#define	MT_ARPHRD_IEEE802		6		/* IEEE 802.2 Ethernet/TR/TB.  */
#define	MT_ARPHRD_ARCNET		7		/* ARCnet.  */
#define	MT_ARPHRD_APPLETLK		8		/* APPLEtalk.  */
#define	MT_ARPHRD_DLCI			15		/* Frame Relay DLCI.  */
#define	MT_ARPHRD_ATM			19		/* ATM.  */
#define	MT_ARPHRD_METRICOM		23		/* Metricom STRIP (new IANA id).  */
#define MT_ARPHRD_IEEE1394		24		/* IEEE 1394 IPv4 - RFC 2734.  */
#define MT_ARPHRD_EUI64			27		/* EUI-64.  */
#define MT_ARPHRD_INFINIBAND	32		/* InfiniBand.  */

/* Dummy types for non ARP hardware */
#define MT_ARPHRD_SLIP			256
#define MT_ARPHRD_CSLIP			257
#define MT_ARPHRD_SLIP6			258
#define MT_ARPHRD_CSLIP6		259
#define MT_ARPHRD_RSRVD			260		/* Notional KISS type.  */
#define MT_ARPHRD_ADAPT			264
#define MT_ARPHRD_ROSE			270
#define MT_ARPHRD_X25			271		/* CCITT X.25.  */
#define MT_ARPHRD_HWX25			272		/* Boards with X.25 in firmware.  */
#define MT_ARPHRD_PPP			512
#define MT_ARPHRD_CISCO			513		/* Cisco HDLC.  */
#define MT_ARPHRD_HDLC		ARPHRD_CISCO
#define MT_ARPHRD_LAPB			516		/* LAPB.  */
#define MT_ARPHRD_DDCMP			517		/* Digital's DDCMP.  */
#define	MT_ARPHRD_RAWHDLC		518		/* Raw HDLC.  */

#define MT_ARPHRD_TUNNEL		768		/* IPIP tunnel.  */
#define MT_ARPHRD_TUNNEL6		769		/* IPIP6 tunnel.  */
#define MT_ARPHRD_FRAD			770     /* Frame Relay Access Device.  */
#define MT_ARPHRD_SKIP			771		/* SKIP vif.  */
#define MT_ARPHRD_LOOPBACK		772		/* Loopback device.  */
#define MT_ARPHRD_LOCALTLK		773		/* Localtalk device.  */
#define MT_ARPHRD_FDDI			774		/* Fiber Distributed Data Interface. */
#define MT_ARPHRD_BIF			775     /* AP1000 BIF.  */
#define MT_ARPHRD_SIT			776		/* sit0 device - IPv6-in-IPv4.  */
#define MT_ARPHRD_IPDDP			777		/* IP-in-DDP tunnel.  */
#define MT_ARPHRD_IPGRE			778		/* GRE over IP.  */
#define MT_ARPHRD_PIMREG		779		/* PIMSM register interface.  */
#define MT_ARPHRD_HIPPI			780		/* High Performance Parallel I'face. */
#define MT_ARPHRD_ASH			781		/* (Nexus Electronics) Ash.  */
#define MT_ARPHRD_ECONET		782		/* Acorn Econet.  */
#define MT_ARPHRD_IRDA			783		/* Linux-IrDA.  */
#define MT_ARPHRD_FCPP			784		/* Point to point fibrechanel.  */
#define MT_ARPHRD_FCAL			785		/* Fibrechanel arbitrated loop.  */
#define MT_ARPHRD_FCPL			786		/* Fibrechanel public loop.  */
#define MT_ARPHRD_FCFABRIC		787		/* Fibrechanel fabric.  */
#define MT_ARPHRD_IEEE802_TR 	800		/* Magic type ident for TR.  */
#define MT_ARPHRD_IEEE80211 	801		/* IEEE 802.11.  */
#define MT_ARPHRD_IEEE80211_PRISM 		802	/* IEEE 802.11 + Prism2 header.  */
#define MT_ARPHRD_IEEE80211_RADIOTAP	803	/* IEEE 802.11 + radiotap header.  */
#define MT_ARPHRD_IEEE802154 			804	/* IEEE 802.15.4 header.  */
#define MT_ARPHRD_IEEE802154_PHY 		805	/* IEEE 802.15.4 PHY header.  */
         
#define MT_ARPHRD_VOID	  		0xFFFF	/* Void type, nothing is known.  */
#define MT_ARPHRD_NONE	  		0xFFFE	/* Zero header length.  */

#define	MT_ARPOP_REQUEST	1		/* ARP request.  */
#define	MT_ARPOP_REPLY		2		/* ARP reply.  */
#define	MT_ARPOP_RREQUEST	3		/* RARP request.  */
#define	MT_ARPOP_RREPLY		4		/* RARP reply.  */
#define	MT_ARPOP_InREQUEST	8		/* InARP request.  */
#define	MT_ARPOP_InREPLY	9		/* InARP reply.  */
#define	MT_ARPOP_NAK		10		/* (ATM)ARP NAK.  */

struct myArpHdr{
	bit16 ar_hrd;		/* Format of hardware address.  */
	bit16 ar_pro;		/* Format of protocol address.  */
	bit8  ar_hln;			/* Length of hardware address.  */
	bit8  ar_pln;			/* Length of protocol address.  */
	bit16 ar_op;		/* ARP opcode (command).  */
};

struct	MYARP{
	struct myArpHdr  ea_hdr;	/* fixed-size header */
	bit8 arp_sha[6];		/* sender hardware address */
	bit8 arp_spa[4];		/* sender protocol address */
	bit8 arp_tha[6];		/* target hardware address */
	bit8 arp_tpa[4];		/* target protocol address */
};







#define MT_IPPROTO_IP		0
#define MT_IPPROTO_ICMP		1	
#define MT_IPPROTO_IGMP		2
#define MT_IPPROTO_IPIP		4
#define MT_IPPROTO_TCP		6
#define MT_IPPROTO_EGP		8
#define MT_IPPROTO_PUP		12
#define MT_IPPROTO_UDP		17
#define MT_IPPROTO_IDP		22
#define MT_IPPROTO_TP		29
#define MT_IPPROTO_DCCP		33
#define MT_IPPROTO_IPV6		41
#define MT_IPPROTO_RSVP		46
#define MT_IPPROTO_GRE		47
#define MT_IPPROTO_ESP		50
#define MT_IPPROTO_AH		51
#define MT_IPPROTO_MTP		92
#define MT_IPPROTO_BEETPH	94
#define MT_IPPROTO_ENCAP	98
#define MT_IPPROTO_PIM		103
#define MT_IPPROTO_COMP		108
#define MT_IPPROTO_SCTP		132
#define MT_IPPROTO_UDPLITE	136
#define MT_IPPROTO_RAW		255

struct MYIP{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	bit32 ihl:4;
	bit32 version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	bit32 version:4;
	bit32 ihl:4;
#else
# error	"Please fix <bits/endian.h>"
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






/* Types */
#define MT_ICMP_ECHOREPLY		0	/* Echo Reply			*/
#define MT_ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define MT_ICMP_SOURCE_QUENCH	4	/* Source Quench		*/
#define MT_ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define MT_ICMP_ECHO			8	/* Echo Request			*/
#define MT_ICMP_TIME_EXCEEDED	11	/* Time Exceeded		*/
#define MT_ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define MT_ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define MT_ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		*/
#define MT_ICMP_INFO_REQUEST	15	/* Information Request		*/
#define MT_ICMP_INFO_REPLY		16	/* Information Reply		*/
#define MT_ICMP_ADDRESS		17	/* Address Mask Request		*/
#define MT_ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define MT_NR_ICMP_TYPES		18

/* Codes for UNREACH. */
#define MT_ICMP_NET_UNREACH	0	/* Network Unreachable		*/
#define MT_ICMP_HOST_UNREACH	1	/* Host Unreachable		*/
#define MT_ICMP_PROT_UNREACH	2	/* Protocol Unreachable		*/
#define MT_ICMP_PORT_UNREACH	3	/* Port Unreachable		*/
#define MT_ICMP_FRAG_NEEDED	4	/* Fragmentation Needed/DF set	*/
#define MT_ICMP_SR_FAILED		5	/* Source Route failed		*/
#define MT_ICMP_NET_UNKNOWN	6
#define MT_ICMP_HOST_UNKNOWN	7
#define MT_ICMP_HOST_ISOLATED	8
#define MT_ICMP_NET_ANO		9
#define MT_ICMP_HOST_ANO		10
#define MT_ICMP_NET_UNR_TOS	11
#define MT_ICMP_HOST_UNR_TOS	12
#define MT_ICMP_PKT_FILTERED	13	/* Packet filtered */
#define MT_ICMP_PREC_VIOLATION	14	/* Precedence violation */
#define MT_ICMP_PREC_CUTOFF	15	/* Precedence cut off */
#define MT_NR_ICMP_UNREACH		15	/* instead of hardcoding immediate value */
       
/* Codes for REDIRECT. */
#define MT_ICMP_REDIR_NET		0	/* Redirect Net			*/
#define MT_ICMP_REDIR_HOST		1	/* Redirect Host		*/
#define MT_ICMP_REDIR_NETTOS	2	/* Redirect Net for TOS		*/
#define MT_ICMP_REDIR_HOSTTOS	3	/* Redirect Host for TOS	*/

/* Codes for TIME_EXCEEDED. */
#define MT_ICMP_EXC_TTL		0	/* TTL count exceeded		*/
#define MT_ICMP_EXC_FRAGTIME	1	/* Fragment Reass time exceeded	*/


struct MYICMP{
	bit8  icmp_type;			/* type of message, see below */
	bit8  icmp_code;			/* type sub code */
	bit16 icmp_cksum;			/* ones complement checksum of struct */
	union{
		bit8   ih_pptr;				/* ICMP_PARAMPROB */
		struct in_addr ih_gwaddr;	/* gateway address */
		struct ih_idseq{			/* echo datagram */
			bit16 icd_id;
			bit16 icd_seq;
		} ih_idseq;
		bit32  ih_void;

		/* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
		struct ih_pmtu{
			bit16 ipm_void;
			bit16 ipm_nextmtu;
		} ih_pmtu;

		struct ih_rtradv{
			bit16 irt_num_addrs;
			bit16 irt_wpa;
			bit16 irt_lifetime;
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
			bit32 its_otime;
			bit32 its_rtime;
			bit32 its_ttime;
		} id_ts;
		struct{
			struct ip idi_ip;
			/* options and then 64 bits of data */
		} id_ip;
		struct icmp_ra_addr id_radv;
		bit32 id_mask;
		bit8  id_data[1];
	} icmp_dun;
#define	icmp_otime	icmp_dun.id_ts.its_otime
#define	icmp_rtime	icmp_dun.id_ts.its_rtime
#define	icmp_ttime	icmp_dun.id_ts.its_ttime
#define	icmp_ip		icmp_dun.id_ip.idi_ip
#define	icmp_radv	icmp_dun.id_radv
#define	icmp_mask	icmp_dun.id_mask
#define	icmp_data	icmp_dun.id_data
};



struct MYTCP{
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
#  error "Adjust your <bits/endian.h> defines"
# endif
	bit16 window;
	bit16 check;
	bit16 urg_ptr;
};




struct MYUDP{
	bit16 source;
	bit16 dest;
	bit16 len;
	bit16 check;
}; 





struct MYDNS{
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
# 	error "Adjust your <bits/endian.h> defines"
# endif
		};
		bit16 flags;
	};
	bit16 qdcnt;
	bit16 ancnt;
	bit16 nscnt;
	bit16 arcnt;
};





struct dns_answer{
	bit16 name;
	bit16 type;
	bit16 cls;
	bit32 ttl;
	bit16 len;
	bit8  addr[4];
};








#endif
