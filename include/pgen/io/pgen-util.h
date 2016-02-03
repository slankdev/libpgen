
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



#ifndef PGEN_UTIL_H
#define PGEN_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int pgen_send_to_pcapng(FILE* fp, const void* buf, int buflen);
int pgen_readblock_from_pcapng(FILE* fp, void* buf, int buflen);
int pgen_recv_from_pcapng(FILE* fp, void* buf, int buflen);

int pgen_recv_from_pcap(FILE*, void*, int);
int pgen_send_to_pcap(FILE*, const void*, int);

int pgen_recv_from_pcapng(FILE*, void*, int);
int pgen_send_to_pcapng(FILE*, const void*, int);

void pgen_hex(const void* data, int len);

unsigned short checksum(unsigned short *data, int len);
unsigned short checksumTcp(struct ip_header, struct tcp_header, const void*, int);
unsigned short checksumUdp(struct ip_header, struct udp_header, const void*, int);



#endif /* PGEN_UTIL_H */
