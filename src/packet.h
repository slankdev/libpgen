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
#define PGEN_MAX_EXT_DATA_LEN 98000



class pgen_packet{
	public:
		int 	len;
		int 	ext_data_len;
		u_char 	data[PGEN_MAX_PACKET_LEN];
		u_char  ext_data[PGEN_MAX_EXT_DATA_LEN];
		
		pgen_packet();
		virtual void clear()=0;
		virtual void compile()=0;
		virtual void cast(const u_char*, const int)=0;
		virtual void send(const char* ifname)=0;
		virtual void summary()=0;
		virtual void info()=0;	
		
		void hex();
		int  length();
		void addData(const u_char* , int );
		void compile_addData(); 
		void send_handle(pgen_t*);
};





class pgen_eth : public pgen_packet {
	protected:
		struct MYETH eth; 
	public:
		static const int minLen = sizeof(struct MYETH);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			macaddr dst;
			macaddr src;
			int type;
		}ETH;
		
		pgen_eth();
		pgen_eth(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L2(ifname);}
		void send_L2(const char* ifname);
		void summary();
		void info();
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
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L2(ifname);}
		void summary();
		void info();
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
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void send_L3(const char* ifname);
		void summary();
		void info();
};




class pgen_icmp : public pgen_ip {
	protected:
		struct MYICMP icmp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYICMP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{	
			int option;
			int code;
			int id;
			int seq;
		}ICMP;
		
		pgen_icmp();
		pgen_icmp(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();

 };






class pgen_tcp : public pgen_ip {
	protected:
		struct MYTCP tcp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYTCP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int src;
			int dst;
			struct{
				char fin:1;
				char syn:1;
				char rst:1;
				char psh:1;
				char ack:1;
				char urg:1;
			}flags;
			int window;
			int seq;
			int ack;
		}TCP;

		pgen_tcp();
		pgen_tcp(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
};




class pgen_udp : public pgen_ip {
	protected:
		struct MYUDP udp;
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYUDP);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			int src;
			int dst;
			bit16 len;
		}UDP;

		pgen_udp();
		pgen_udp(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
};




#define MAX_QUERY  16
#define MAX_ANSWER 16
#define MAX_AUTH   16
#define MAX_ADD    16
class pgen_dns :public pgen_udp {
	protected:
		struct MYDNS dns;
		bit8  query_data[256];
		bit32 query_data_len;
		bit8  answer_data[256];
		bit32 answer_data_len;
		bit8  auth_data[256];
		bit32 auth_data_len;
		bit8  addition_data[256];
		bit32 addition_data_len;

	public:
		static const int minLen = pgen_udp::minLen+DNS_HDR_LEN;
		static const int maxLen = PGEN_MAX_PACKET_LEN; 
		struct{
			u_int16_t id;
			struct{
				bit8 qr:1;
				bit8 opcode:4;
				bit8 aa:1;
				bit8 tc:1;
				bit8 rd:1;
				bit8 ra:1;
				bit8 nouse:3;
				bit8 rcode:4;
			}flags;
			u_int16_t qdcnt;
			u_int16_t ancnt;
			u_int16_t nscnt;
			u_int16_t arcnt;
			struct{
				std::string name;
				u_int16_t type;
				u_int16_t cls;
			}query[MAX_QUERY];
			struct{
				bit16  name;
				bit16  type;
				bit16  cls;
				bit32  ttl;
				bit16  len;
				
				//ipaddr addr;
				//std::string url;
				bit8 data[32];
			}answer[MAX_ANSWER];
			struct{
				bit16 name;
				bit16 type;
				bit16 cls;
				bit16 ttl;
				bit16 len;
				
				//ipaddr addr;
				//std::string url;
				bit8  data[32];
			}auth[MAX_AUTH];
			struct{
				bit16 name;
				bit16 type;
				bit16 cls;
				bit16 ttl;
				bit16 len;
				
				//ipaddr addr;
				//std::string url;
				bit8  data[32];
			}addition[MAX_ADD];
		}DNS;

		pgen_dns();
		pgen_dns(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void debug();
		
		void clear_query();
		void clear_answer();
		void clear_auth();
		void clear_addition();
		void compile_query();
		void compile_answer();
		void compile_auth();
		void compile_addition();
		int  cast_query(const u_char*, int);
		int  cast_answer(const u_char*, int);
		int  cast_auth(const u_char* packet, int len);
		int  cast_addition(const u_char* packet, int len);

};



class pgen_ardrone : public pgen_udp {
	public:
		static const int minLength = pgen_udp::minLen+39; // minimum ardrone packet
		static const int macLength = PGEN_MAX_PACKET_LEN;
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
		void cast(const u_char*, const int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();

		void DSUMMARY();
};




class pgen_dhcp : public pgen_udp {
	public:
		static const int minLen = pgen_udp::minLen+DNS_HDR_LEN;
		static const int maxLen = PGEN_MAX_PACKET_LEN; 
		struct{
			int a;
		}DHCP;
		
		pgen_dhcp();
		pgen_dhcp(const u_char*, int);
		void clear();
		void compile();
		void cast(const u_char*, const int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
};





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
		pgen_unknown(const u_char*, int);
		void clear();
		bool cast(const u_char*, int);
		void summary();

		void hex();
		bool ipaddris(ipaddr addr);
		bool macaddris(macaddr addr);
		bool portis(unsigned short port);
};





#endif /* PACKET_H */
