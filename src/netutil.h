#ifndef NETUTIL_H
#define NETUTIL_H

void pgen_sendpacket(const char*, const u_char*, int);
int sendPacketL2_test(const char* dev, const u_char* packet, int len);


const char* port2service(int port, int protocol);
int sendRawPacket(int sock, const u_char* data, int len, 
		int layer, struct sockaddr* sap);
int initRawSocket_old(const char* dev, int layer);
int initRawSocket(const char* dev, int promisc, int overIp);


unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);


#endif /* NETUTIL_H */
