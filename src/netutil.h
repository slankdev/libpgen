#ifndef NETUTIL_H
#define NETUTIL_H

bool pgen_sendpacket_L2(const char*, const u_char*, int);
bool pgen_sendpacket_L3(const char*, const u_char*, int);
int sendRawPacket(int sock, const u_char* data, int len, 
		int layer, struct sockaddr* sap);

const char* port2service(int port, int protocol);

int initRawSocket_old(const char* dev, int layer);
int initRawSocket(const char* dev, int promisc, int overIp);

unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


#endif /* NETUTIL_H */
