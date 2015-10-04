#ifndef UDP_H
#define UDP_H


#include "ip.h"

class pgen_udp : public pgen_ip {
	protected:
		struct udp_header udp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct udp_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int src;
			int dst;
			bit16 len;
		}UDP;

		pgen_udp();
		pgen_udp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();
};




#endif
