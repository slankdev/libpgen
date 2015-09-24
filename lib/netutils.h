

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#ifndef NETUTIL_H
#define NETUTIL_H

#include "pgen.h"
#include "pgen-types.h"
#include "address.h"



pgen_t* pgen_open(const char* dev, void*);
pgen_t* pgen_open_offline(const char*, int mode);
void pgen_close(pgen_t* p);


void sniff(pgen_t* handle, bool (*callback)(const u_char*, int));


int pgen_sendpacket_handle(pgen_t* p, const u_char* packet, int len);
int pgen_sendpacket_L2(const char*, const u_char*, int);
int pgen_sendpacket_L3(const char*, const u_char*, int, struct sockaddr*);


char* pgen_port2service(int port, int protocol);
unsigned short checksum(unsigned short *data, int len);
unsigned short checksumTcp(const u_char* dp, int datalen);
unsigned short checksumUdp(const u_char* dp, int datalen);


int initRawSocket(const char* dev, int promisc, int overIp);


int getmacaddr_test(const char *ifname, char *macaddrstr);



int pgen_getipbydev(const char* dev, char* ip);
int pgen_getmaskbydev(const char* dev, char* ip);
int pgen_getmacbydev(const char* dev, char* mac);

#endif /* NETUTIL_H */
