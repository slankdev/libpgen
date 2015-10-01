
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
#include "pgen-netutil.h"

#include "pgen.h"
#include "pgen-io.h"
#include "packet.h"
#include "pgen-error.h"

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
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>
#include <ifaddrs.h>
#else
#include <netpacket/packet.h>
#endif




int pgen_send_to_netif(int fd, const void* buf, int len){
	return -2;	
}





int pgen_recv_from_netif(int fd, void* buf, int len){
	int recv_len;
	u_char  buf0[4096];
	u_char* p = buf0;

	recv_len = read(fd, p, 4096);
	if(recv_len < 0){
		perror("pgen_recv");
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_READ;
		return recv_len;
	}

#ifndef __linux

	struct bpf_hdr *bpfhdr;
	bpfhdr = (struct bpf_hdr*)p;
	p += bpfhdr->bh_hdrlen;
	recv_len = bpfhdr->bh_caplen;

#endif /* __linux */

	memcpy(buf, p, recv_len);
	return recv_len;	
}






int open_bpf(const char* dev, int promisc) {
#ifdef __linux
	
	pgen_errno2 = PG_ERRNO_NOSUPPORT;
	return -1;

#else /* __linux */
	int sock;	
	struct ifreq ifr;
	const unsigned int one  = 1;

	/* BPFデバイスファイルのオープン */
	int i;
	char buf[256];
	for (i = 0; i < 4; i++) { 
		snprintf(buf, 255, "/dev/bpf%d", i);
		if ((sock = open(buf, O_RDWR)) > 0)
			break;
	}

	if (i >= 5) {
		fprintf(stderr, "cannot open BPF\n");
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_OPENBPF;
		return -1;
	}
	
	// set buffer size
	int bufsize = 4096;
	if (ioctl(sock, BIOCSBLEN, &bufsize) < 0) {
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_SETBUF;
		close(sock);
		return -1;
	}

	// bind to device
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	if(ioctl(sock, BIOCSETIF, &ifr) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_BIND;
		close(sock);
		return -1;
	}

	// set promisc
	if(promisc){
		if (ioctl(sock, BIOCPROMISC, NULL) < 0) {
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNOBSD_PROMISC;
			close(sock);
			return -1;
		}
	}

	//if recv packet then call read fast
	if (ioctl(sock, BIOCIMMEDIATE, &one) < 0) {
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_IMDAT;
		close(sock);
		return -1;
	}

	// set recv sendPacket 
	if (ioctl(sock, BIOCSSEESENT, &one) < 0) {
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_RCVALL;
		close(sock);
			return -1;
	}

	// flush recv buffer
	if (ioctl(sock, BIOCFLUSH, NULL) < 0) {
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_FLUSH;
		close(sock);
		return -1;
	}

	// no complite src macaddr
	if (ioctl(sock, BIOCSHDRCMPLT, &one) < 0) {
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNOBSD_NCMPMAC;
		close(sock);
		return -1;
	}

	return sock;

#endif /* __linux */
}






int initRawSocket(const char* dev, int promisc, int overIp){
	int sock;
	const unsigned int one  = 1;
	

	if(overIp){
#ifdef __linux
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
#else
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
#endif
		if(sock < 0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_SOCKET;
			return -1;
		}
	
		if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_HDRINC;
			close(sock);
			return -1;
		}

#ifdef __linux
		if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, dev, sizeof(dev))<0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_BIND;
			close(sock);
			return -1;	
		}
#endif

	}else{  // over ethernet

#ifdef __linux
		struct ifreq ifreq;
		struct sockaddr_ll sa;

		sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(sock < 0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_SOCKET;
			return -1;
		}

		// get interface index number
		memset(&ifreq, 0, sizeof(struct ifreq));
		strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
		if(ioctl(sock, SIOCGIFINDEX, &ifreq) < 0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_BIND;
			close(sock);
			return -1;
		}

		// bind to device
		sa.sll_family = PF_PACKET;
		sa.sll_protocol = htons(ETH_P_ALL);
		sa.sll_ifindex  = ifreq.ifr_ifindex;
		if(bind(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_BIND;
			close(sock);
			return -1;
		}


		if(promisc){
			if(ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0)	{
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_PROMISC;
				close(sock);
				return -1;
			}
			ifreq.ifr_flags = ifreq.ifr_flags|IFF_PROMISC;
			if(ioctl(sock, SIOCSIFFLAGS, &ifreq) < 0){
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_PROMISC;
				close(sock);
				return -1;
			}
		}

#else	// for bsd

		sock = open_bpf(dev, promisc);

#endif
	
	}
	return sock;
}








int pgen_getipbydev(const char* dev, char* ip){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;

	if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_SOCKET;
		return -1;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
	if(ioctl(sockd, SIOCGIFADDR, &ifr) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_IOCTL;
		close(sockd);
		return -1;
	}
	close(sockd);
	sa = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(ip, inet_ntoa(sa->sin_addr));
	
	return 1;
}





int pgen_getmaskbydev(const char* dev, char* ip){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;

	if((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_SOCKET;
		return -1;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
	if(ioctl(sockd, SIOCGIFNETMASK, &ifr) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_IOCTL;
		close(sockd);
		return -1;
	}
	close(sockd);
	sa = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(ip, inet_ntoa(sa->sin_addr));

	return 1;
}







int pgen_getmacbydev(const char* dev, char* mac){
#ifdef __linux

	int sockd;
	struct ifreq ifr;
	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_SOCKET;
		return -1;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
	if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_IOCTL;
		close(sockd);
		return -1;
	}
	close(sockd);
	sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[0], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[1], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[2], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[3], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[4], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[5] );
	return 1;

#else // for bsd
    
	struct ifaddrs *ifap, *ifaptr;
    unsigned char *ptr;
	if(getifaddrs(&ifap) != 0){
		return -1;	
	}
	for(ifaptr = ifap; ifaptr != NULL; ifaptr = (ifaptr)->ifa_next) {
		if (!strcmp((ifaptr)->ifa_name, dev) && 
				(((ifaptr)->ifa_addr)->sa_family == AF_LINK)) {
			ptr = (unsigned char *)LLADDR((struct sockaddr_dl *)(ifaptr)->ifa_addr);
			sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
								*ptr, *(ptr+1), *(ptr+2), 
								*(ptr+3), *(ptr+4), *(ptr+5));
			freeifaddrs(ifap);
			return 1;
		}
	}
	return -1;

#endif
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



