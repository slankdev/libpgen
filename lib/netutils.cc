

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

#include "pgen.h"
#include "netutils.h"
#include "packet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#ifndef __linux
#else
#include <netpacket/packet.h>
#endif



#ifdef __linux

#endif 





// mode 0: read , 1: write
pgen_t* pgen_open_offline(const char* filename, int mode){
	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	handle->is_offline = 1;
	
	switch(mode){
		case 0: // read mode
			handle->is_write = 0;
			handle->is_read  = 1;
			handle->offline.fd = fopen(filename, "rb");
			if(handle->offline.fd == NULL){
				perror("pgen_open_offline");
				pgen_close(handle);
				handle = NULL;
			}
			if(fread(&handle->offline.filehdr,sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				perror("pgen_open_offline");
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		case 1: // write mode 
			handle->is_write = 1;
			handle->is_read  = 0;
			handle->offline.fd = fopen(filename, "wb");
			if(handle->offline.fd == NULL){
				perror("pgen_open_offline");
				pgen_close(handle);
				handle = NULL;
			}
			handle->offline.filehdr.magic = 0xa1b2c3d4;
			handle->offline.filehdr.version_major = 0x0002;
			handle->offline.filehdr.version_minor = 0x0004;
			handle->offline.filehdr.timezone = 0x0000;
			handle->offline.filehdr.sigfigs  = 0x0000;
			handle->offline.filehdr.snaplen  = 0x0000ffff;
			handle->offline.filehdr.linktype = 0x00000001;
			if(fwrite(&handle->offline.filehdr,sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				perror("pgen_open_writepcap");
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		default: // mode not found
			fprintf(stderr, "pgen_open_offline: mode not found\n");
			pgen_close(handle);
			break;
	}

	return handle;	
}




pgen_t* pgen_open(const char* dev, void* nouseyet){
	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	
	handle->is_write = 1;
	handle->is_read  = 1;

	handle->fd = initRawSocket(dev, 1, 0);
	if(handle->fd < 0){
		perror("pgen_open");
		handle =  NULL;
	}

	return handle;
}



void pgen_close(pgen_t* p){
	if(p->is_offline == 1){
		fclose(p->offline.fd);
	}else{
		close(p->fd);
	}

	free(p);
	return ;	
}



void sniff(pgen_t* handle, bool (*callback)(const u_char*, int)){
	if(handle->is_read == 0){
		fprintf(stderr, "sniff: handle is not read mode \n");
		return;
	}


	u_char packet[20000];
	bool result = true;
	int len;
	

	for(;result;){
		if(handle->is_offline == 1){ // offline sniff
			struct pcap_pkthdr hdr;
			if(fread(&hdr, sizeof(struct pcap_pkthdr),1,handle->offline.fd) < 1){
				perror("sniff");
				//fprintf(stderr, "sniff: file is finish\n");
				return;
			}
			if(fread(packet, hdr.len, 1, handle->offline.fd) <= 0){
				perror("sniff");
				return;
			}
			len = hdr.len;
				
		}else{ // online sniff	
			if((len = read(handle->fd, packet, sizeof(packet))) < 0){
				perror("sniff_handle");
				return;
			}
		}
		result = (*callback)(packet, len);
	}
}







int pgen_sendpacket_handle(pgen_t* p, const u_char* packet, int len){
	if(p->is_write == 0){
		fprintf(stderr, "pgen_sendpacket_handle: this handle is not write mode \n");
		return -1;
	}
	int sendlen;
	
	if(p->is_offline == 1){
		struct pcap_timeval ts_now;
		gettimeofday((struct timeval*)&ts_now, NULL);

		struct pcap_pkthdr pkthdr;
		pkthdr.ts = ts_now;
		pkthdr.caplen = len;
		pkthdr.len    = len;

		if(fwrite(&pkthdr, sizeof(struct pcap_pkthdr), 1, p->offline.fd) < 1){
			perror("pgen_sendpacket_handle");
			sendlen = -1;
		}else{
			if(fwrite(packet, len, 1, p->offline.fd) < 1){
				perror("pgen_sendpacket_handle");
				sendlen = -1;
			}
		}
		sendlen = len;
	
	}else{
		sendlen = write(p->fd, packet, len);
		if(sendlen < 0){
			perror("pgen_sendpacket_handle");
		}
	}
	
	return sendlen;
}



int pgen_sendpacket_L3(const char* dev, const u_char* packet, int len, struct sockaddr* sa){
	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 1)) < 0){
		fprintf(stderr, "sendPacketL3\n");
		return -1;
	}
	sendlen = sendto(sock, packet, len, 0, sa, sizeof(struct sockaddr));
	if(sendlen < 0){
		perror("pgen_sendpacket_L2");
	}

	close(sock);
	return sendlen;
}




int pgen_sendpacket_L2(const char* dev, const u_char* packet, int len){
	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 0)) < 0){
		fprintf(stderr, "sendPacketL2\n");
		return -1;
	}
	sendlen = write(sock, packet, len);
	if(sendlen < 0){
		perror("pgen_sendpacket_L2");
	}

	close(sock);
	return sendlen;
}




char* pgen_port2service(int port, int protocol){
	static char str[16];
	struct servent* serv;
	if(protocol == 1){
		serv = getservbyport(htons(port), "tcp");
	}else if(protocol == 2){
		serv = getservbyport(htons(port), "udp");
	}else{
		fprintf(stderr, "pgen_port2service: second argument is not support\n");
		strncpy(str, "error", sizeof(str)-1);
		return str;
	}

	if(serv == NULL){
		strncpy(str, "not-found", sizeof(str)-1);
		return str;
	}else{
		strncpy(str, serv->s_name, sizeof(str)-1);
		return str;
	}
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






unsigned short checksumTcp(const u_char *dp, int datalen){
	struct tcp_header tcp;
	struct ip_header ip;
	char data[100];
	memcpy(&ip, dp, IP_HDR_LEN);
	dp += IP_HDR_LEN;
	memcpy(&tcp, dp, TCP_HDR_LEN);
	dp += TCP_HDR_LEN;
	memcpy(data, dp, datalen-TCP_HDR_LEN-IP_HDR_LEN);
	dp += datalen-TCP_HDR_LEN-IP_HDR_LEN;
	
	struct tpack {
	  struct ip_header ip;
	  struct tcp_header tcp;
	  u_char data[100];
	}p;
	
	memcpy(&p.ip, &ip, IP_HDR_LEN);
	memcpy(&p.tcp , &tcp, TCP_HDR_LEN);
	memcpy(&p.data, data, datalen-TCP_HDR_LEN-IP_HDR_LEN);

	p.ip.ttl = 0;
	p.ip.protocol  = IPPROTO_TCP;
	p.ip.saddr = ip.saddr;
	p.ip.daddr = ip.daddr;
	p.ip.check    = htons((sizeof p.tcp) );

#define PHLEN 12
	p.tcp.check = 0;
	return checksum((u_int16_t*)&p.ip.ttl, PHLEN+datalen-IP_HDR_LEN);
}







int initRawSocket(const char* dev, int promisc, int overIp){
#ifndef __linux
	fprintf(stderr, "initRawSocket: this function is not implement in BSD yet\n");
	return -1;
#else
	int sock;
	
	if(overIp){
			sock=socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
			if(sock < 0){
				perror("initRawSocket: ");
				return -1;
			}

			if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, dev, sizeof(dev))<0){
				close(sock);
				return -1;	
			}
	}else{
		
		struct ifreq ifreq;
		struct sockaddr_ll sa;

		sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(sock < 0){
			perror("initRawSocket");
			return -1;
		}

		memset(&ifreq, 0, sizeof(struct ifreq));
		strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
		if(ioctl(sock, SIOCGIFINDEX, &ifreq) < 0){
			perror("initRawSocket");
			close(sock);
			return -1;
		}
		sa.sll_family = PF_PACKET;
		sa.sll_protocol = htons(ETH_P_ALL);
		sa.sll_ifindex  = ifreq.ifr_ifindex;
		if(bind(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0){
			perror("initRawSocket");
			close(sock);
			return -1;
		}

		/*
		int yes = 1;
		if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) < 0){
			perror("initRawSocket");
			close(sock);
			return -1;
		}
		*/

		if(promisc){
			if(ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0)	{
				perror("initRawSocket");
				close(sock);
				return -1;
			}
			ifreq.ifr_flags = ifreq.ifr_flags|IFF_PROMISC;
			if(ioctl(sock, SIOCSIFFLAGS, &ifreq) < 0){
				perror("initRawSocket");
				close(sock);
				return -1;
			}
		}
	
	}

	return sock;
#endif
}




#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
int getmacaddr_test(const char *ifname, char *macaddrstr) {
    struct ifaddrs *ifap, *ifaptr;
    unsigned char *ptr;

    if (getifaddrs(&ifap) == 0) {
        for(ifaptr = ifap; ifaptr != NULL; ifaptr = (ifaptr)->ifa_next) {
            if (!strcmp((ifaptr)->ifa_name, ifname) && (((ifaptr)->ifa_addr)->sa_family == AF_LINK)) {
                ptr = (unsigned char *)LLADDR((struct sockaddr_dl *)(ifaptr)->ifa_addr);
                sprintf(macaddrstr, "%02x:%02x:%02x:%02x:%02x:%02x",
                                    *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
                break;
            }
        }
        freeifaddrs(ifap);
        return ifaptr != NULL;
    } else {
        return 0;
    }
}
