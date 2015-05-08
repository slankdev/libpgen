#ifndef PGEN_PACK_H
#define PGEN_PACK_H

/* include must!!! */
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "pgen-variable.h"
//--saiteigenn-zettai-hituyou---
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//------------------------------


#include "mytypes.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>		/* for struct sockaddr_in */
class pgen_packet{
	protected:
		int 	packetType;
		int 	len;
		u_char 	data[PGEN_PACKLEN];
	public:
		
		pgen_packet();
		virtual void clear();
		virtual void info()=0;	
		virtual void wrap(const char* ifname)=0;
		virtual void sendPack(const char* ifname)=0;
		void hex();
		void hexFull();
};



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ethernet.h> 		/* for struct ether_header */
class pgen_eth : public pgen_packet {
	protected:
		struct ether_header eth;
	public:
		macaddr	eth_srcEth;
		macaddr 	eth_dstEth;
		int 		eth_type;

		struct{
			macaddr src;
			macaddr dst;
		}ETH;
		
		pgen_eth();
		void clear();
		void info();
		void wrap(const char* ifname);
		void sendPack(const char* ifname);
};




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>	/* for struct ether_arp 	*/
class pgen_arp : public pgen_eth {
	protected:
		struct ether_arp 	arp;
	public:
		int arp_option;
		macaddr	arp_srcEth;
		macaddr	arp_dstEth;
		ipaddr	arp_srcIp;
		ipaddr	arp_dstIp;

		struct{
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		void clear();
		void info();
		void wrap(const char* ifname);
		void sendPack(const char* ifname);
};



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>			/* for struct iphdr			*/
class pgen_ip : public pgen_eth {
	protected:
		struct iphdr		ip;
	public:
		ipaddr 	ip_srcIp;
		ipaddr 	ip_dstIp;
		int			ip_type;
		int ip_tos; 
		int ip_id; 
		int ip_ttl; 
		
		struct{
			ipaddr src;
			ipaddr dst;
			int tos; 
			int id; 
			int ttl; 
		}IP;


		pgen_ip();
		void clear();
		void info();
		void wrap(const char* ifname);
		void wrapLite(const char* ifname);
		void sendPack(const char* ifname);
};




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>	/* for struct icmp			*/
class pgen_icmp : public pgen_ip {
	protected:
		struct icmp icmp;
		u_char data[100]; // no use yet
	public:
		int icmp_option;
		int icmp_code;

		struct{	
			int option;
			int code;
		}ICMP;
		
		pgen_icmp();
		void clear();
		void info();
		void wrap(const char* ifname);
		void wrapLite(const char* ifname);
		void sendPack(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
 };






#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>		// for struct tcp		
class pgen_tcp : public pgen_ip {
	protected:
		struct tcphdr tcp;
		u_char data[100]; // no use yet
	public:
		struct{
			int srcPort;
			int dstPort;
			struct{
				char fin;
				char syn;
				char rst;
				char psh;
				char ack;
				char urg;
			}frag ;
			int window;
			int seqNum;
			int ackNum;
		}TCP;

		pgen_tcp();
		void clear();
		void info();
		void wrap(const char* ifname);
		void sendPack(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
};






#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/udp.h>		// for struct udp		
class pgen_udp : public pgen_ip {
	protected:
		struct udphdr udp;
		u_char data[100]; // no use yet
	public:
		int udp_srcPort;
		int udp_dstPort;
		
		struct{
			int srcPort;
			int dstPort;
		}UDP;

		pgen_udp();
		void clear();
		void info();
		void wrap(const char* ifname);
		void wrapLite(const char* ifname);
		void sendPack(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
};





#endif
