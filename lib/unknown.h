
#ifndef UNKNOWN_H
#define UNKNOWN_H



#include "address.h"






class pgen_unknown{
	private:
		bit8 data[PGEN_MAX_PACKET_LEN];
	public:
		int len;
		bool isETH;
		bool isARP;
		bool isIP;
		bool isICMP;
		bool isTCP;
		bool isUDP;
		struct{
			macaddr src;
			macaddr dst;
		}ETH;
		struct{
			ipaddr src;
			ipaddr dst;
		}IP;
		struct{
			bit16 src;
			bit16 dst;
		}TCP;
		struct{
			bit16 src;
			bit16 dst;
		}UDP;

		pgen_unknown();
		pgen_unknown(const void*, int);
		void clear();
		void send_handle(pgen_t*);
		int  cast(const void*, int);
		void summary();

		void hex();
		bool ipaddris(ipaddr addr);
		bool macaddris(macaddr addr);
		bool portis(unsigned short port);
};



#endif
