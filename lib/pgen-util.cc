
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




#include "pgen-util.h"
#include "pgen-types.h"
#include "pgen-error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

#ifndef __linux
#include <net/bpf.h>
#endif






int pgen_send_to_pcap(FILE* fp, const void* buf, int len){
	int sendlen = 0;
	struct timeval ts_now;
	struct pcap_pkthdr pkthdr;
	gettimeofday(&ts_now, NULL);
	
	pkthdr.ts.tv_sec = (unsigned int)(ts_now.tv_sec);
	pkthdr.ts.tv_usec = (unsigned int)(ts_now.tv_usec);
	pkthdr.caplen = len;
	pkthdr.len    = len;

	if(fwrite(&pkthdr, sizeof(struct pcap_pkthdr), 1, fp) != 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FWRITE;
		sendlen = -1;
	}else{
		if(fwrite(buf, len, 1, fp) < 1){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_FWRITE;
			sendlen = -1;
		}
		sendlen = len;
	}
	
	return sendlen;	
}






int pgen_recv_from_pcap(FILE* fp, void* buf, int len){
	struct pcap_pkthdr hdr;
	if(fread(&hdr, sizeof(struct pcap_pkthdr), 1, fp) < 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FREAD;
		return -1;
	}
	if(fread(buf, hdr.len, 1, fp) != 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FREAD;
		return -1;
	}

	return hdr.len;
}





unsigned short checksum(unsigned short *data, int len){
  u_int32_t sum = 0; 

  for (; len > 1; len -= 2) {
    sum += *data++;
    if (sum & 0x80000000) 
      sum = (sum & 0xffff) + (sum >> 16);
  }

  if (len == 1) {
    u_int16_t i = 0;
    *(u_char*) (&i) = *(u_char *) data;
    sum += i;
  }

  while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);

  return ~sum;
}




// not test
unsigned short checksumTcp_new(struct ip_header ip, 
			struct tcp_header tcp, const void* buf, int total_len){
	
	int ih_len = 12;
	struct{
		bit32 ip_src;	
		bit32 ip_dst;
		bit8  zero;
		bit8  ip_protocol;
		bit16 ip_length;
		struct tcp_header tcp;
	}buffer;

	memset(&buffer, 0, sizeof buffer);
	buffer.ip_src = ip.saddr;
	buffer.ip_dst = ip.daddr;
	buffer.ip_protocol = 0x06;
	buffer.ip_length = htons(total_len);
	memcpy(&buffer.tcp, &tcp, tcp.doff*4);

	return checksum((u_int16_t*)&buffer, ih_len+total_len);
}


unsigned short checksumTcp(const u_char *dp, int datalen){
	struct tcp_header tcp;
	struct ip_header ip;
	char data[100];
	memcpy(&ip, dp, 20);  // this is bug
	dp += 20;
	memcpy(&tcp, dp, sizeof tcp);
	dp += 20;
	memcpy(data, dp, datalen-20-20); // this is bug
	dp += datalen-20-20;
	
	struct tpack {
	  struct ip_header ip;
	  struct tcp_header tcp;
	  u_char data[100];
	}p;
	
	memcpy(&p.ip, &ip, 20);
	memcpy(&p.tcp , &tcp, 20);
	memcpy(&p.data, data, datalen-20-20);

	p.ip.ttl = 0;
	p.ip.protocol  = 0x06;
	p.ip.saddr = ip.saddr;
	p.ip.daddr = ip.daddr;
	p.ip.check    = htons((sizeof p.tcp) );

#define PHLEN 12
	p.tcp.check = 0;
	return checksum((u_int16_t*)&p.ip.ttl, PHLEN+datalen-20);
}






unsigned short checksumUdp(const u_char *dp, int datalen){
	return 0;
}



