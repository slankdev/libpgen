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
		pgent_mac	eth_srcEth;
		pgent_mac 	eth_dstEth;
		int 		eth_type;
		
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
		pgent_mac	arp_srcEth;
		pgent_mac	arp_dstEth;
		pgent_ip	arp_srcIp;
		pgent_ip	arp_dstIp;

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
		pgent_ip 	ip_srcIp;
		pgent_ip 	ip_dstIp;
		int			ip_type;
		
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
	public:
		int icmp_option;
		int icmp_code;

		pgen_icmp();
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
#include <netinet/tcp.h>		// for struct tcp		
class pgen_tcp : public pgen_ip {
	protected:
		struct tcphdr tcp;
	public:
		int tcp_srcPort;
		int tcp_dstPort;
		struct{
			char fin;
			char syn;
			char rst;
			char psh;
			char ack;
			char urg;
		}tcp_frag ;
		int tcp_window;
		int tcp_doff;

		pgen_tcp();
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
#include <netinet/udp.h>		// for struct udp		
class pgen_udp : public pgen_ip {
	protected:
		struct udphdr udp;
	public:
		int udp_srcPort;
		int udp_dstPort;

		pgen_udp();
		void clear();
		void info();
		void wrap(const char* ifname);
		void wrapLite(const char* ifname);
		void sendPack(const char* ifname);
};





#endif
