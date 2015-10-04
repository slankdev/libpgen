#ifndef TCP_H
#define TCP_H

#include "ip.h"

class pgen_tcp : public pgen_ip {
	protected:
		struct tcp_header tcp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct tcp_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int src;
			int dst;
			struct{
				u_char fin:1;
				u_char syn:1;
				u_char rst:1;
				u_char psh:1;
				u_char ack:1;
				u_char urg:1;
			}flags;
			int window;
			int seq;
			int ack;
		}TCP;

		pgen_tcp();
		pgen_tcp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();
};




#endif
