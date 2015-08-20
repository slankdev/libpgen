#ifndef NETUTIL_H
#define NETUTIL_H

#include "mytypes.h"
#include "address.h"



void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc);

int  pgen_sendpacket_L2(const char*, const u_char*, int);
int  pgen_sendpacket_L3(const char*, const u_char*, int, struct sockaddr*);

const char* port2service(int port, int protocol);
unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


#endif /* NETUTIL_H */
