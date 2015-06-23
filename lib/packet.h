#ifndef PGEN_PACK_H
#define PGEN_PACK_H


#include "pgen.h"
#include "packconf.h"
#include "pgen-funcs.h"
#include "address.h"
#include "mytypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>


#define PGEN_PACKLEN 2000





class pgen_packet{
	protected:
		int 	packetType;
		int 	len;
		u_char 	data[PGEN_PACKLEN];
	public:
		
		pgen_packet();
		virtual void CLEAR();
		virtual void INFO()=0;	
		virtual void WRAP()=0;
		virtual void SEND(const char* ifname)=0;
		virtual void SUMMARY()=0;
		void hex();
		void hexFull();
};





//#include <net/ethernet.h> 		/* for struct ether_header */
class pgen_eth : public pgen_packet {
	protected:
		struct MYETH eth;
	public:
		struct{
			int type;
			macaddr src;
			macaddr dst;
		}ETH;
		
		pgen_eth();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
};




//#include <netinet/if_ether.h>	/* for struct ether_arp 	*/
class pgen_arp : public pgen_eth {
	protected:
		struct MYARP arp;
	public:
		struct{
			int option;
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
};



//#include <netinet/ip.h>			/* for struct iphdr			*/
class pgen_ip : public pgen_eth {
	protected:
		struct MYIP		ip;
	public:
		struct{
			int type;
			ipaddr src;
			ipaddr dst;
			int tos; 
			int id; 
			int ttl; 
		}IP;


		pgen_ip();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
};




//#include <netinet/ip_icmp.h>	/* for struct icmp			*/
class pgen_icmp : public pgen_ip {
	protected:
		struct MYICMP icmp;
		u_char _data[100]; // no use yet
	public:
		struct{	
			int option;
			int code;
			int id;
			int seq;
		}ICMP;
		
		pgen_icmp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
		void SUMMARY();
 };






//#include <netinet/tcp.h>		// for struct tcp		
class pgen_tcp : public pgen_ip {
	protected:
		struct MYTCP tcp;
		u_char _data[100]; // no use yet
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
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
};






//#include <netinet/udp.h>		// for struct udp		
class pgen_udp : public pgen_ip {
	protected:
		struct MYUDP udp;
		u_char _data[100]; // no use yet
	public:
		struct{
			int srcPort;
			int dstPort;
		}UDP;

		pgen_udp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void setData(const u_char* p, int len); // no use yet
};



class pgen_dns :public pgen_udp {
	protected:
		struct MYDNSHDR dns;
	public:
		struct{
			u_int16_t id;
			u_int16_t flags;
			u_int16_t qdcnt;
			u_int16_t ancnt;
			u_int16_t nscnt;
			u_int16_t arcnt;
			std::string name;
			u_int16_t type;
			u_int16_t cls;
		}DNS;

		pgen_dns();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
};







#endif
