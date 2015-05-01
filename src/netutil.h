#ifndef NETUTIL_H
#define NETUTIL_H


int sendRaw(int sock);
int initRawSocket(const char* dev, int ipOnly, int promisc);


#endif
