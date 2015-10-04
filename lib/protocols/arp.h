#ifndef ARP_H
#define ARP_H


#include "eth.h"


class pgen_arp : public pgen_eth {
	protected:
		struct arp_packet arp;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct arp_packet);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int operation;
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		pgen_arp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int);
		void send(const char* ifname){send_L2(ifname);}
		void summary();
		void info();
		void help();
};


#endif 
