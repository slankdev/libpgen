//#include <linux/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <netinet/tcp.h>
#include <netinet/ip.h>


static u_int16_t CSUM(u_int16_t *data, int len){
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

unsigned short checksumTcp(struct tcphdr tcp, struct iphdr ip, int datalen){
	struct tpack {
	  struct ip ip;
	  struct tcphdr tcp;
	};
	struct tpack p;
	u_int16_t ret;
	
	memcpy(&p.tcp, &tcp, sizeof(struct tcphdr));;
	p.ip.ip_ttl = 0;
	p.ip.ip_p      = IPPROTO_TCP;
	p.ip.ip_src.s_addr = ip.saddr;
	p.ip.ip_dst.s_addr = ip.daddr;
	p.ip.ip_sum    = htons((sizeof p.tcp) );

#define PHLEN 12
	p.tcp.th_sum = 0;
	ret = CSUM((u_int16_t*)&p.ip.ip_ttl, PHLEN+sizeof(p.tcp));
	
	return ret;
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


int initRawSocket(const char* dev, int layer){
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
int initRawSocket(const char* dev, int ipOnly, int promisc){
	int sock;
	struct ifreq ifreq;
	struct sockaddr_ll sa;
	

	



	return sock;	
}*/
