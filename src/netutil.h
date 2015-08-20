#ifndef NETUTIL_H
#define NETUTIL_H

#include "mytypes.h"
#include "address.h"

struct sniff_filter{
	struct{
		bool ipsrc;
		bool ipdst;
		bool ethsrc;
		bool ethdst;
	}flag;
	ipaddr ipsrc;
	ipaddr ipdst;
	macaddr ethsrc;
	macaddr ethdst;
};


void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc);
// this functions 's test is not try yet
void sniff_with_filter(const char* dev, bool (*callback)(const u_char*, int), 
									int promisc, struct sniff_filter* filter);


int  pgen_sendpacket_L2(const char*, const u_char*, int);
int  pgen_sendpacket_L3(const char*, const u_char*, int, struct sockaddr*);

int initRawSocket(const char* dev, int promisc, int overIp);

const char* port2service(int port, int protocol);
unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


#endif /* NETUTIL_H */
