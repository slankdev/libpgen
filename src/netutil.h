#ifndef NETUTIL_H
#define NETUTIL_H

#include "pgen.h"
#include "mytypes.h"
#include "address.h"



pgen_t* pgen_open(const char* dev, int promisc, void*);
void pgen_close(pgen_t* p);


void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc);
void sniff(pgen_t* handle, bool (*callback)(const u_char*, int));


int pgen_sendpacket_handle(pgen_t* p, const u_char* packet, int len);
int pgen_sendpacket_L2(const char*, const u_char*, int);
int pgen_sendpacket_L3(const char*, const u_char*, int, struct sockaddr*);


char* pgen_port2service(int port, int protocol);
unsigned short checksum(unsigned short *data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


int initRawSocket(const char* dev, int promisc, int overIp);

#endif /* NETUTIL_H */
