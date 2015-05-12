
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

#include "pgen.h"
#include "pgen-funcs.h"





char* pgen_getIP(const char* ifname){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	char* ipstr;

	if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket()!");
		exit(-1);
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	ioctl(sockd, SIOCGIFADDR, &ifr);
	close(sockd);
	sa = (struct sockaddr_in*)&ifr.ifr_addr;
	ipstr = inet_ntoa(sa->sin_addr);

	return ipstr;
}




char* pgen_getMASK(const char* ifname){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	char* maskstr;

	if((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(-1);
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	ioctl(sockd, SIOCGIFNETMASK, &ifr);
	close(sockd);
	sa = (struct sockaddr_in*)&ifr.ifr_addr;
	maskstr = inet_ntoa(sa->sin_addr);

	return maskstr;
}





char* pgen_getMAC( const char* ifname){
	int sockd;
	struct ifreq ifr;
	char* macstr;
	u_char addr[6];

	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket()!");
		exit(-1);
	}
	macstr = (char*)malloc(sizeof(char)*19);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	ioctl(sockd, SIOCGIFHWADDR, &ifr);
	close(sockd);
	for(int i=0; i<6; i++)
		addr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];
	snprintf(macstr ,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
		*addr, *(addr+1), *(addr+2), *(addr+3), *(addr+4), *(addr+5));

	return macstr;
}




void pgen_checkPrivileges(void){
	if(0 != getuid()){
		fprintf(stderr, "pgen : requirs root privileges\n");
		exit(-1);
	}
}


