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


unsigned short checksumTcp(const void* data, int len){
	unsigned long sum = 0;
	unsigned short* buf = (unsigned short*)data;
	unsigned short oddbyte;

	while (len > 1) {
		sum += *buf++;
		len -= 2;
	}

	if (len == 1){
		oddbyte = 0;
		*((unsigned short*)&oddbyte) = *(unsigned short*)buf;
		sum += oddbyte;
	}


	sum = (sum & 0xffff) + (sum >> 16);
	sum += (sum >> 16);
	return (unsigned short)~sum;
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
