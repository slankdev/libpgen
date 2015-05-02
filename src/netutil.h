#ifndef NETUTIL_H
#define NETUTIL_H


unsigned short checksumTcp(const void* data, int len);
int sendRawPacket(int sock, const u_char* data, int len, 
		int layer, struct sockaddr* sap);
int initRawSocket(const char* dev, int layer);
//int initRawSocket(const char* dev, int ipOnly, int promisc);


#endif
