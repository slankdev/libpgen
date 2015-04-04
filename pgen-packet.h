#ifndef PGEN_PACK_H
#define PGEN_PACK_H

/* include must!!! */
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "util.h"
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
		pgen_t packetType;
		int sock;
		int len;
		u_char data[PGEN_PACKLEN];
	public:
		pgent_mac eth_srcEth;
		pgent_mac eth_dstEth;
		int eth_type;
		pgent_ip ip_srcIp;
		pgent_ip ip_dstIp;
		int ip_type;

		int tcp_srcPort;
		int tcp_dstPort;
		int udp_srcPort;
		int udp_dstPort;
		
		int srcPort;
		int dstPort;
		
		
		
		pgen_packet();
		~pgen_packet(); 
		virtual void info()=0;	
		virtual void compile(const char* ifname);
		virtual void clear();
		void hex();
		void sendDl(const char* ifname);
		void sendNw(const char* ifname);
		void sendTp(const char* ifname);
		bool eth_srcEth_isEmpty();
		bool eth_dstEth_isEmpty();
		bool ip_srcIp_isEmpty();
		bool ip_dstIp_isEmpty();
		bool tcp_srcPort_isEmpty();
		bool tcp_dstPort_isEmpty();
		bool udp_srcPort_isEmpty();
		bool udp_dstPort_isEmpty();
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
	private:
		struct ether_header eth;
	public:
		void compile(const char* ifname);
		void info();
};






#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ethernet.h> 		/* for struct ether_header 	*/
#include <netinet/if_ether.h>	/* for struct ether_arp 	*/
class pgen_arp : public pgen_packet {
	private:
		struct ether_header eth;
		struct ether_arp arp;
	public:
		int arp_option;
		pgent_mac arp_srcEth;
		pgent_mac arp_dstEth;
		pgent_ip arp_srcIp;
		pgent_ip arp_dstIp;
		pgen_arp();
		void info();
		void clear();
		void compile(const char* ifname);
		bool arp_srcEth_isEmpty();
		bool arp_dstEth_isEmpty();
		bool arp_srcIp_isEmpty();
		bool arp_dstIp_isEmpty();
};





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ethernet.h>		/* for struct ether_header	*/ 
#include <netinet/ip.h>			/* for struct iphdr			*/
class pgen_ip : public pgen_packet {
	private:
		struct ether_header eth;
		struct iphdr ip;
	public:
		void compile(const char* ifname);
		void info();
};






#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ethernet.h> 		/* for struct ether_header	*/
#include <netinet/ip.h>			/* for struct iphdr			*/
#include <netinet/ip_icmp.h>	/* for struct icmp			*/
class pgen_icmp : public pgen_packet {
	private:
		struct ether_header eth;
		struct iphdr ip;
		struct icmphdr icmp;
	public:
		int icmp_option;
		int icmp_code;

		pgen_icmp();
		void info();
		void clear();
		void compile(const char* ifname);
 };


#endif
