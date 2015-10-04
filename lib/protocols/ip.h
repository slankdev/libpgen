#ifndef IP_H
#define IP_H

#include <eth.h>


class pgen_ip : public pgen_eth {
	protected:
		struct ip_header		ip;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct ip_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			bit8  tos;
			bit16 tot_len;
			bit16 id;
			bit16 frag_off;
			bit8  ttl; 
			bit8  protocol;
			ipaddr src;
			ipaddr dst;
		}IP;


		pgen_ip();
		pgen_ip(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L3(ifname);}
		void send_L3(const char* ifname);
		void summary();
		void info();
		void help();
};




#endif 
