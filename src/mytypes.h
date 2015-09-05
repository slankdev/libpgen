#ifndef MYTYPES_H
#define MYTYPES_H


#include <endian.h>		// for BYTE_ORDER



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
struct pgen_opt{};


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
#  error "Adjust your <bits/endian.h> defines"
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




//#define	MT_ETHERTYPE_IP			0x0800		/* IP */
//#define	MT_ETHERTYPE_ARP		0x0806		/* Address resolution */
//#define	MT_ETHERTYPE_REVARP		0x8035		/* Reverse ARP */
//#define	MT_ETHERTYPE_AARP		0x80F3		/* AppleTalk ARP */
//#define	MT_ETHERTYPE_VLAN		0x8100		/* IEEE 802.1Q VLAN tagging */
//#define	MT_ETHERTYPE_IPV6		0x86dd		/* IP protocol version 6 */

//#define	MT_ARPHRD_NETROM		0		/* From KA9Q: NET/ROM pseudo. */
//#define	MT_ARPHRD_ETHER			1		/* Ethernet 10/100Mbps.  */
//#define	MT_ARPHRD_EETHER		2		/* Experimental Ethernet.  */
//#define	MT_ARPHRD_IEEE802		6		/* IEEE 802.2 Ethernet/TR/TB.  */

//#define	MT_ARPOP_REQUEST	1		/* ARP request.  */
//#define	MT_ARPOP_REPLY		2		/* ARP reply.  */
//#define	MT_ARPOP_RREQUEST	3		/* RARP request.  */
//#define	MT_ARPOP_RREPLY		4		/* RARP reply.  */
//#define	MT_ARPOP_InREQUEST	8		/* InARP request.  */
//#define	MT_ARPOP_InREPLY	9		/* InARP reply.  */
//#define	MT_ARPOP_NAK		10		/* (ATM)ARP NAK.  */

//#define MT_IPPROTO_IP			0
//#define MT_IPPROTO_ICMP		1	
//#define MT_IPPROTO_IGMP		2
//#define MT_IPPROTO_IPIP		4
//#define MT_IPPROTO_TCP		6
//#define MT_IPPROTO_UDP		17
//#define MT_IPPROTO_IDP		22
//#define MT_IPPROTO_TP			29
//#define MT_IPPROTO_DCCP		33
//#define MT_IPPROTO_IPV6		41
//#define MT_IPPROTO_ESP		50
//#define MT_IPPROTO_MTP		92
//#define MT_IPPROTO_UDPLITE	136
//#define MT_IPPROTO_RAW		255

/* Types */
//#define MT_ICMP_ECHOREPLY		0	/* Echo Reply			*/
//#define MT_ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
//#define MT_ICMP_SOURCE_QUENCH	4	/* Source Quench		*/
//#define MT_ICMP_REDIRECT		5	/* Redirect (change route)	*/
//#define MT_ICMP_ECHO			8	/* Echo Request			*/
//#define MT_ICMP_TIME_EXCEEDED	11	/* Time Exceeded		*/
//#define MT_ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
//#define MT_ICMP_TIMESTAMP		13	/* Timestamp Request		*/
//#define MT_ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		*/
//#define MT_ICMP_INFO_REQUEST	15	/* Information Request		*/
//#define MT_ICMP_INFO_REPLY		16	/* Information Reply		*/
//#define MT_ICMP_ADDRESS		17	/* Address Mask Request		*/
//#define MT_ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
//#define MT_NR_ICMP_TYPES		18

/* Codes for UNREACH. */
//#define MT_ICMP_NET_UNREACH	0	/* Network Unreachable		*/
//#define MT_ICMP_HOST_UNREACH	1	/* Host Unreachable		*/
//#define MT_ICMP_PROT_UNREACH	2	/* Protocol Unreachable		*/
//#define MT_ICMP_PORT_UNREACH	3	/* Port Unreachable		*/
//#define MT_ICMP_FRAG_NEEDED	4	/* Fragmentation Needed/DF set	*/
//#define MT_ICMP_SR_FAILED		5	/* Source Route failed		*/
//#define MT_ICMP_NET_UNKNOWN	6
//#define MT_ICMP_HOST_UNKNOWN	7
//#define MT_ICMP_HOST_ISOLATED	8
//#define MT_ICMP_NET_ANO		9
//#define MT_ICMP_HOST_ANO		10
//#define MT_ICMP_NET_UNR_TOS	11
//#define MT_ICMP_HOST_UNR_TOS	12
//#define MT_ICMP_PKT_FILTERED	13	/* Packet filtered */
//#define MT_ICMP_PREC_VIOLATION	14	/* Precedence violation */
//#define MT_ICMP_PREC_CUTOFF	15	/* Precedence cut off */
//#define MT_NR_ICMP_UNREACH		15	/* instead of hardcoding immediate value */
/* Codes for REDIRECT. */
//#define MT_ICMP_REDIR_NET		0	/* Redirect Net			*/
//#define MT_ICMP_REDIR_HOST		1	/* Redirect Host		*/
//#define MT_ICMP_REDIR_NETTOS	2	/* Redirect Net for TOS		*/
//#define MT_ICMP_REDIR_HOSTTOS	3	/* Redirect Host for TOS	*/
/* Codes for TIME_EXCEEDED. */
//#define MT_ICMP_EXC_TTL		0	/* TTL count exceeded		*/
//#define MT_ICMP_EXC_FRAGTIME	1	/* Fragment Reass time exceeded	*/


#endif /* MYTYPES_H */
