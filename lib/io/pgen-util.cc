
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




#include <pgen/pgen-types.h>
#include <pgen/io/pgen-util.h>
#include <pgen/io/pgen-error.h>

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
	struct pgen_pcap_pkthdr pkthdr;
	gettimeofday(&ts_now, NULL);
	
	pkthdr.ts.tv_sec = (unsigned int)(ts_now.tv_sec);
	pkthdr.ts.tv_usec = (unsigned int)(ts_now.tv_usec);
	pkthdr.caplen = len;
	pkthdr.len    = len;

	if(fwrite(&pkthdr, sizeof(struct pgen_pcap_pkthdr), 1, fp) != 1){
		pgen_errno_native = errno;
		pgen_errno = PG_NERRNO_FWRITE;
		sendlen = -1;
	}else{
		if(fwrite(buf, len, 1, fp) < 1){
			pgen_errno_native = errno;
			pgen_errno = PG_NERRNO_FWRITE;
			sendlen = -1;
		}
		sendlen = len;
	}
	
	return sendlen;	
}






int pgen_recv_from_pcap(FILE* fp, void* buf, int len){
	struct pgen_pcap_pkthdr hdr;
	if(fread(&hdr, sizeof(struct pgen_pcap_pkthdr), 1, fp) != 1){
		pgen_errno_native = errno;
		pgen_errno = PG_NERRNO_FREAD;
		if(feof(fp) == 0)	return 0;
		else 				return -1;
	}
	if(fread(buf, hdr.len, 1, fp) != 1){
		pgen_errno_native = errno;
		pgen_errno = PG_NERRNO_FREAD;
		if(feof(fp) == 0)	return 0;
		else 				return -1;
	}

	return hdr.len;
}







void pgen_hex(const void* d, int len){
	printf("hexdump len: %d \n", len);

	const u_char* data = (const u_char*)d;

	int row=0;
	int column=0;
	for(row=0; (row+column)<16+len; row+=16){
		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf("   ");
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
			}else{
				if((row+column)%16 == 0) printf("%04x:    ", row+column);
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
				printf("%02x ", data[row+column]);
			}
		}

		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf(" ");
			}else{
				if((row+column)%16 == 0) 
					printf("  ");
				if((row+column)%8 == 0 && (row+column)%16!=0) 
					printf("  ");
				
				if(0x20<=data[row+column] && data[row+column]<=0x7E)
					printf("%c", data[row+column]);
				else
					printf(".");
				
				if((row+column+1)%16 == 0)	
					printf("\n");
			}
		}
	}
	printf("\n");
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
unsigned short checksumTcp(struct ip_header ip, 
			struct tcp_header tcp, const void* data, int total_len){
	
	
	struct{
		bit32 ip_src;	
		bit32 ip_dst;
		bit8  zero;
		bit8  ip_protocol;
		bit16 ip_length;
		struct tcp_header tcp;
	}headers;
	u_char buffer0[10000];
	u_char* p = buffer0;


	memset(&headers, 0, sizeof headers);
	memset(&buffer0, 0, sizeof buffer0);
	headers.ip_src = ip.saddr;
	headers.ip_dst = ip.daddr;
	headers.ip_protocol = 0x06;
	headers.ip_length = htons(total_len);
	memcpy(&headers.tcp, &tcp, tcp.doff*4);

	memcpy(p, &headers, 12+tcp.doff*4);
	p += 12+tcp.doff*4;
	memcpy(p, data, total_len-tcp.doff*4);

	return checksum((u_int16_t*)buffer0, 12+total_len);
}





unsigned short checksumUdp(struct ip_header ip, 
			struct udp_header udp, const void* data, int total_len){


	struct{
		bit32 ip_src;	
		bit32 ip_dst;
		bit8  zero;
		bit8  ip_protocol;
		bit16 ip_length;
		struct udp_header udp;
	}headers;
	u_char buffer0[10000];
	u_char* p = buffer0;
	

	memset(&headers, 0, sizeof headers);
	memset(&buffer0, 0, sizeof buffer0);

	headers.ip_src = ip.saddr;
	headers.ip_dst = ip.daddr;
	headers.ip_protocol = 0x11;
	headers.ip_length = htons(total_len);
	memcpy(&headers.udp, &udp, sizeof(udp));
	

	memcpy(p, &headers, sizeof(headers));
	p += sizeof(headers);
	memcpy(p, data, total_len-sizeof(udp));

	return checksum((u_int16_t*)buffer0, total_len+sizeof(headers));



	return 0;
}



