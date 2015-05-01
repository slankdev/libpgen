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


int sendRaw(int sock){
	return 1;
}




int initRawSocket(const char* dev, int ipOnly, int promisc){
	int sock;
	struct ifreq ifreq;
	struct sockaddr_ll sa;
	
	//if(ipOnly)	sock=socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if(ipOnly)	sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	else		sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

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
	if(ipOnly)	sa.sll_protocol = htons(ETH_P_IP);
	else		sa.sll_protocol = htons(ETH_P_ALL);
	sa.sll_ifindex = ifreq.ifr_ifindex;

	if(bind(sock, (struct sockaddr*)&sa, sizeof sa) < 0){
		perror("initRawSocket bind: ");
		close(sock);
		return -1;
	}

	if(promisc){
		if(ioctl(sock, SIOCGIFFLAGS, &ifreq) < 0){
			perror("initRawSocket ioctl: ");
			close(sock);
			return -1;
		}
		ifreq.ifr_flags=ifreq.ifr_flags | IFF_PROMISC;
		if(ioctl(sock, SIOCSIFFLAGS, &ifreq) < 0){	
			perror("initRawSocket ioctl: ");
			close(sock);
			return -1;
		}
	}

	return sock;	
}
