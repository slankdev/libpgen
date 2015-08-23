#ifndef PACKET_H
#define PACKET_H


#include "pgen.h"
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


#define PGEN_MAX_PACKET_LEN 10000
#define PGEN_MAX_EXT_DATA_LEN 1000



class pgen_packet{
	public:
		int 	len;
		int 	ext_data_len;
		u_char 	data[PGEN_MAX_PACKET_LEN];
		u_char  ext_data[PGEN_MAX_EXT_DATA_LEN];
		
		pgen_packet();
		virtual void clear()=0;
		virtual void info()=0;	
		virtual void summary()=0;
		virtual void compile()=0;
		virtual void send(const char* ifname)=0;
		virtual void cast(const u_char*, const int)=0;
	
		void hex();

		void compile_addData();
		
		
		bool addData(const u_char* , int );
		void _addData_WRAP();
};





class pgen_eth : public pgen_packet {
	protected:
		struct MYETH eth; 
	public:
		static const int minLen = sizeof(struct MYETH);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int type;
			macaddr src;
			macaddr dst;
		}ETH;
		
		pgen_eth();
		pgen_eth(const u_char*, int);
		void clear();
		void info();
		void summary();
		void compile();
		void send(const char* ifname);
		void cast(const u_char*, const int len);
};




class pgen_arp : public pgen_eth {
	protected:
		struct MYARP arp;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct MYARP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int operation;
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		pgen_arp(const u_char*, int);
		void clear();
		void info();
		void compile();
		void send(const char* ifname);
		void summary();
		void cast(const u_char*, const int len);
};



class pgen_ip : public pgen_eth {
	protected:
		struct MYIP		ip;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct MYIP);
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
		pgen_ip(const u_char*, int);
		void clear();
		void info();
		void summary();
		void compile();
		void send(const char* ifname);
		void cast(const u_char*, const int len);
};




class pgen_icmp : public pgen_ip {
	protected:
		struct MYICMP icmp;
	public:
		static const int minLen = sizeof(struct MYICMP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;//??
		struct{	
			int option;
			int code;
			int id;
			int seq;
		}ICMP;
		
		pgen_icmp();
		pgen_icmp(const u_char*, int);
		void clear();
		void info();
		void compile();
		void send(const char* ifname);
		void summary();
		void cast(const u_char*, const int len);

 };






class pgen_tcp : public pgen_ip {
	protected:
		struct MYTCP tcp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYTCP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;//??
		struct{
			int src;
			int dst;
			struct{
				char fin;
				char syn;
				char rst;
				char psh;
				char ack;
				char urg;
			}flags;
			int window;
			int seq;
			int ack;
		}TCP;

		pgen_tcp();
		pgen_tcp(const u_char*, int);
		void clear();
		void info();
		void compile();
		void send(const char* ifname);
		void summary();
		void cast(const u_char*, const int len);
};




class pgen_udp : public pgen_ip {
	protected:
		struct MYUDP udp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYUDP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;//??
		struct{
			int src;
			int dst;
		}UDP;

		pgen_udp();
		pgen_udp(const u_char*, int);
		void clear();
		void info();
		void summary();
		void compile();
		void send(const char* ifname);
		void cast(const u_char*, const int len);
};





class pgen_dns :public pgen_udp {
	protected:
		struct MYDNS dns;
		bit8 answer[256];
		bit32 answer_len;
	public:
		static const int minLen = pgen_udp::minLen+sizeof(struct MYETH);
		static const int maxLen = PGEN_MAX_PACKET_LEN; //wakanne
		struct{
			u_int16_t id;
			struct{
				bit8 qr;
				bit8 opcode;
				bit8 aa;
				bit8 tc;
				bit8 rd;
				bit8 ra;
				bit8 nouse;
				bit8 rcode;
			}flags;
			u_int16_t qdcnt;
			u_int16_t ancnt;
			u_int16_t nscnt;
			u_int16_t arcnt;
			struct{
				std::string name;
				u_int16_t type;
				u_int16_t cls;
			}query;
			struct{
				bit16  name;
				bit16  type;
				bit16  cls;
				bit32  ttl;
				bit16  len;
				ipaddr addr;
			}answer;
		}DNS;

		pgen_dns();
		pgen_dns(const u_char*, int);
		void clear();
		void info();
		void summary();
		void compile();
		void send(const char* ifname);
		void cast(const u_char*, const int);
		
		void _wrap_query();
		void _wrap_answer();
		void DSUMMARY();
};



class pgen_ardrone : public pgen_udp {
	protected:
		char cmd[256];
		int   clen;
	public:
		static const int minLength = pgen_udp::minLen+39;
		static const int macLength = PGEN_MAX_PACKET_LEN; //???
		struct{
			struct{
				long seq;
				long flag;
				long roll;
				long pitch;
				long gaz;
				struct{
					long x;
					long y;
					long z;
				}yaw;
			}pcmd;
			struct{
				long seq;
				long command;
			}ref;
		}ARDRONE;

		pgen_ardrone();
		pgen_ardrone(const u_char*, int);
		void clear();
		void compile();
		void info();
		void summary();
		void send(const char* ifname);
		void cast(const u_char*, const int);

		void DSUMMARY();
		void _printdata();
};







class pgen_unknown{
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
		pgen_unknown(const u_char*, int);
		void clear();
		void summary();
		bool cast(const u_char*, int);

		bool IPaddris(ipaddr addr);
		bool MACaddris(macaddr addr);
		bool TCPportis(unsigned short port);
		bool UDPportis(unsigned short port);
};





#endif /* PACKET_H */
