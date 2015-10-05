

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




#ifndef PGEN_NETUTIL_H
#define PGEN_NETUTIL_H



int pgen_send_to_netif(int, const void*, int);
int pgen_recv_from_netif(int, void*, int);
int pgen_recv_from_netif_to(int, void*, int, struct timeval);

int open_bpf(const char* dev, int promisc);
int initRawSocket(const char* dev, int promisc, int overIp);


int   pgen_getipbydev(const char* dev, char* ip);
int   pgen_getmaskbydev(const char* dev, char* ip);
int   pgen_getmacbydev(const char* dev, char* mac);
char* pgen_port2service(int port, const char* protocol, char* buf);




#endif /* PGEN_NETUTIL_H */
