
#ifndef ETH_H
#define ETH_H


#include "packet.h"
#include "address.h"


class pgen_eth : public pgen_packet {
	protected:
		struct ethernet_header eth; 
	public:
		static const int minLen = sizeof(struct ethernet_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			macaddr dst;
			macaddr src;
			bit16 type;
		}ETH;
		
		pgen_eth();
		pgen_eth(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L2(ifname);}
		void send_L2(const char* ifname);
		void summary();
		void info();
		void help();
};


#endif /* ETH_H */
