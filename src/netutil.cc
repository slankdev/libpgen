
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

#include "pgen.h"
#include "netutil.h"








void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc){
	u_char packet[20000];
	bool result = true;
	int len;
	int sock;
	
	if((sock=initRawSocket(dev, promisc, 0))<0){
		perror("sniff");
		return;
	}

	for(;result;){
		if((len = read(sock, packet, sizeof(packet))) < 0){
			perror("read");
			return;
		}
		result = (*callback)(packet, len);
	}
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


