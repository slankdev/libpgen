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

#define PGEN_PACKLEN 16384
#define PGEN_ADDDATALEN 1000


class pgen_unknown{
	protected:
		bool _isETH;
		bool _isARP;
		bool _isIP;
		bool _isICMP;
		bool _isTCP;
		bool _isUDP;
		int len;
	public:
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
		pgen_unknown(const bit8*, int);
		void CLEAR();
		void SUMMARY();
		bool CAST(const bit8*, int);
		bool isETH();
		bool isARP();
		bool isIP();
		bool isICMP();
		bool isTCP();
		bool isUDP();
};





class pgen_packet{
	protected:
		int 	len;
		int 	additionalLen;
		u_char 	data[PGEN_PACKLEN];
		u_char  additionalData[PGEN_ADDDATALEN];

	public:
		pgen_packet();
		virtual void help()=0;
		virtual void CLEAR()=0;
		virtual void INFO()=0;	
		virtual void WRAP()=0;
		virtual void SEND(const char* ifname)=0;
		virtual void CAST(const bit8*, const int)=0;
		void hex();
		void hexFull();
		char* TOBYTE();
		bool addData(const char* , int );
		int length(){  return len;	}
		void _addData_WRAP();
};





//#include <net/ethernet.h> 		/* for struct ether_header */
class pgen_eth : public pgen_packet {
	protected:
		struct MYETH eth; 
	public:
		static const int minLen = sizeof(struct MYETH);
		static const int maxLen = PGEN_PACKLEN;
		struct{
			int type;
			macaddr src;
			macaddr dst;
		}ETH;
		
		pgen_eth();
		pgen_eth(const bit8*, int);
		void CLEAR();
		void INFO();
		void SUMMARY();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, const int len);
};




//#include <netinet/if_ether.h>	/* for struct ether_arp 	*/
class pgen_arp : public pgen_eth {
	protected:
		struct MYARP arp;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct MYARP);
		static const int maxLen = PGEN_PACKLEN;
		struct{
			int operation;
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		pgen_arp(const bit8*, int);
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, const int len);
};



//#include <netinet/ip.h>			/* for struct iphdr			*/
class pgen_ip : public pgen_eth {
	protected:
		struct MYIP		ip;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct MYIP);
		static const int maxLen = PGEN_PACKLEN;
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
		pgen_ip(const bit8*, int);
		void CLEAR();
		void INFO();
		void SUMMARY();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, const int len);
};




//#include <netinet/ip_icmp.h>	/* for struct icmp			*/
class pgen_icmp : public pgen_ip {
	protected:
		struct MYICMP icmp;
		u_char _data[100]; // no use yet
	public:
		static const int minLen = sizeof(struct MYICMP);
		static const int maxLen = PGEN_PACKLEN;//??
		struct{	
			int option;
			int code;
			int id;
			int seq;
		}ICMP;
		
		pgen_icmp();
		pgen_icmp(const bit8*, int);
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, const int len);

 };






//#include <netinet/tcp.h>		// for struct tcp		
class pgen_tcp : public pgen_ip {
	protected:
		struct MYTCP tcp;
		u_char _data[100]; // no use yet
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYTCP);
		static const int maxLen = PGEN_PACKLEN;//??
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
		pgen_tcp(const bit8*, int);
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, const int len);
};




//#include <netinet/udp.h>		// for struct udp		
class pgen_udp : public pgen_ip {
	protected:
		struct MYUDP udp;
		u_char _data[100]; // no use yet
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct MYUDP);
		static const int maxLen = PGEN_PACKLEN;//??
		struct{
			int src;
			int dst;
		}UDP;

		pgen_udp();
		pgen_udp(const bit8*, int);
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, const int len);
};



class pgen_dns :public pgen_udp {
	protected:
		struct MYDNS dns;
		bit8 answer[256];
		bit32 answer_len;
	public:
		static const int minLen = pgen_udp::minLen+sizeof(struct MYETH);
		static const int maxLen = PGEN_PACKLEN; //wakanne
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
		pgen_dns(const bit8*, int);
		void CLEAR();
		void INFO();
		void SUMMARY();
		void DSUMMARY();
		void WRAP();
		void _wrap_query();
		void _wrap_answer();
		void SEND(const char* ifname);
		void CAST(const bit8*, const int);
};



class pgen_ardrone : public pgen_udp {
	protected:
		char cmd[256];
		int   clen;
	public:
		static const int minLength = pgen_udp::minLen+39;
		static const int macLength = PGEN_PACKLEN; //???
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
		pgen_ardrone(const bit8*, int);
		void CLEAR();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, const int);
		void INFO();
		void SUMMARY();
		void _printdata();

		void DSUMMARY();
};






#endif /* PACKET_H */
