#ifndef NETUTIL_H
#define NETUTIL_H

int  pgen_sendpacket_L2(const char*, const u_char*, int);
int  pgen_sendpacket_L3(const char*, const u_char*, int, struct sockaddr*);

int initRawSocket(const char* dev, int promisc, int overIp);

const char* port2service(int port, int protocol);
unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


#endif /* NETUTIL_H */
