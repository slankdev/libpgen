#ifndef NETUTIL_H
#define NETUTIL_H


const char* port2service(int port, int protocol);
unsigned short checksum(const void* data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);
int sendRawPacket(int sock, const u_char* data, int len, 
		int layer, struct sockaddr* sap);
int initRawSocket(const char* dev, int layer);
int initRawSocket_test(const char* dev, int promisc);

int sendPacketL2_test(const char* dev, const u_char* packet, int len);

#endif
