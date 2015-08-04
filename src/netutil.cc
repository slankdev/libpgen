
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netdb.h>

#include "netutil.h"


int sendPacketL2_test(const char* dev, const u_char* packet, int len){
	int sock;
	
	if((sock=initRawSocket(dev, 0, 0)) < 0){
		fprintf(stderr, "sendPacketL2_test\n");
		return -1;
	}

	write(sock, packet, len);
	close(sock);
}



const char* port2service(int port, int protocol){
	struct servent* serv;
	if(protocol == 1)
		serv = getservbyport(htons(port), "tcp");
	else if(protocol == 2)
		serv = getservbyport(htons(port), "udp");
	else
		return "port2service error";


	if(serv == NULL)
		return "not found";
	else
		return serv->s_name;

}




unsigned short checksum(const void* data, int len){
	unsigned long sum = 0;
	unsigned short* buf = (unsigned short*)data;

	while (len > 1) {
		sum += *buf;
		buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum & 0xffff) + (sum >> 16);
	sum = (sum & 0xffff) + (sum >> 16);
	return ~sum;
}



unsigned short checksumTcp(const u_char *dp, int datalen){
	struct tcphdr tcp;
	struct ip ip;
	char data[100];
	memcpy(&ip, dp, sizeof(struct ip));
	dp += sizeof(struct ip);
	memcpy(&tcp, dp, sizeof(struct tcphdr));
	dp += sizeof(struct tcphdr);
	memcpy(data, dp, datalen-sizeof(struct tcphdr)-sizeof(struct ip));
	dp += datalen-sizeof(struct tcphdr)-sizeof(struct ip);
	
	struct tpack {
	  struct ip ip;
	  struct tcphdr tcp;
	  u_char data[100];
	}p;
	
	memcpy(&p.ip, &ip, sizeof(struct ip));
	memcpy(&p.tcp , &tcp, sizeof(struct tcphdr));
	memcpy(&p.data, data, datalen-sizeof(struct tcphdr)-sizeof(struct ip));

	p.ip.ip_ttl = 0;
	p.ip.ip_p      = IPPROTO_TCP;
	p.ip.ip_src.s_addr = ip.ip_src.s_addr;
	p.ip.ip_dst.s_addr = ip.ip_dst.s_addr;
	p.ip.ip_sum    = htons((sizeof p.tcp) );

#define PHLEN 12
	p.tcp.th_sum = 0;
	return checksum((u_int16_t*)&p.ip.ip_ttl, PHLEN+datalen-sizeof(struct ip));
}


int sendRawPacket(int sock, const u_char* data, int len, int layer, struct sockaddr* sap){
	int sendlen;

	switch(layer){
		case 2:
			sendlen = write(sock, data, len);
			if(sendlen < 0){
				perror("sendRawPacket: ");
				return -1;
			}
			break;

		case 3:
			sendlen = sendto(sock, data, len, 0, sap, sizeof(struct sockaddr));
			if(sendlen < 0){
				perror("sendRawPacket: ");
				return -1;
			}
			break;

		default:
			fprintf(stderr, "sendRawPacket: layer no support\n");
			return -1;
			break;
	}	
	return sendlen;
}



int initRawSocket_old(const char* dev, int layer){
	int sock;
	struct ifreq ifreq;
	struct sockaddr_ll sa;
	
	switch(layer){
		case 2:
			sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
			if(sock < 0){
				perror("initRawSocket: ");
				return -1;
			}

			memset(&ifreq, 0, sizeof ifreq);
			strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
			if(ioctl(sock, SIOCGIFINDEX, &ifreq) < 0){
				perror("initRawSocket ioctl: ");
				close(sock);
				return -1;
			}

			sa.sll_family = PF_PACKET;
			sa.sll_protocol = htons(ETH_P_ALL);
			sa.sll_ifindex = ifreq.ifr_ifindex;

			if(bind(sock, (struct sockaddr*)&sa, sizeof sa) < 0){
				perror("initRawSocket bind: ");
				close(sock);
				return -1;
			}
			break;

		case 3:
			sock=socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
			if(sock < 0){
				perror("initRawSocket: ");
				return -1;
			}

			if(setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, dev, sizeof(dev)) < 0){
				return -1;	
			}

			break;

		default:
			fprintf(stderr, "initRawSocket: this layer no support\n");
			return -1;
			break;
	}
	return sock;	
}


/*
int initRawSocket_test(const char* dev, int promisc){
	struct ifreq ifreq;
	struct sockaddr_ll sa;
	int sock;

	if((sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
		perror("InitRawSocket");
		return -1;
	}

	memset(&ifreq, 0, sizeof(struct ifreq));
	strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
	if(ioctl(sock, SIOCGIFINDEX, &ifreq) < 0){
		perror("InitRawSocket");
		close(sock);
		return -1;
	}

	sa.sll_family = PF_PACKET;
	sa.sll_protocol = htons(ETH_P_ALL);
	sa.sll_ifindex  = ifreq.ifr_ifindex;
	if(bind(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0){
		perror("InitRawSocket");
		close(sock);
		return -1;
	}

	if(promisc){
		if(ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0)	{
			perror("InitRawSocket");
			close(sock);
			return -1;
		}
		ifreq.ifr_flags = ifreq.ifr_flags|IFF_PROMISC;
		if(ioctl(sock, SIOCSIFFLAGS, &ifreq) < 0){
			perror("InitRawSocket");
			close(sock);
			return -1;
		}
	}

	return sock;
}
*/


int initRawSocket(const char* dev, int promisc, int overIp){
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
			perror("InitRawSocket");
			return -1;
		}

		memset(&ifreq, 0, sizeof(struct ifreq));
		strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
		if(ioctl(sock, SIOCGIFINDEX, &ifreq) < 0){
			perror("InitRawSocket");
			close(sock);
			return -1;
		}

		sa.sll_family = PF_PACKET;
		sa.sll_protocol = htons(ETH_P_ALL);
		sa.sll_ifindex  = ifreq.ifr_ifindex;
		if(bind(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0){
			perror("InitRawSocket");
			close(sock);
			return -1;
		}

		if(promisc){
			if(ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0)	{
				perror("InitRawSocket");
				close(sock);
				return -1;
			}
			ifreq.ifr_flags = ifreq.ifr_flags|IFF_PROMISC;
			if(ioctl(sock, SIOCSIFFLAGS, &ifreq) < 0){
				perror("InitRawSocket");
				close(sock);
				return -1;
			}
		}
	
	}

	return sock;
}
