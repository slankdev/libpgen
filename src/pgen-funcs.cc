
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





char* pgen_mac2bender(const char* addr){
	FILE *fp;
	const char* filename = "mac_code.list";
	unsigned int  mac[3];
	char* bender;
	char buf[256];
	unsigned int b[3];
	u_char dev_mac[3];
	bender = (char*)malloc(sizeof(char) * (strlen(buf)+1));
	
	if((fp=fopen(filename, "r")) == NULL){
		perror("getbenderbymac fopen");
		strcpy(bender, "error");
		return bender;
	}
	sscanf(addr, "%02x:%02x:%02x", &b[0], &b[1], &b[2]);
	for(int i=0; i<3; i++)	dev_mac[i] = (unsigned char)b[i];
	while(fgets(buf, sizeof(buf), fp) != NULL){
		sscanf(buf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],buf);
		if(mac[0]==dev_mac[0]&&mac[1]==dev_mac[1]&&mac[2]==dev_mac[2]){
			snprintf(bender, strlen(buf), "%s", buf);
			return bender;
		}
		memset(mac, 0, sizeof(mac));
		memset(bender, 0, sizeof(bender));
		memset(buf, 0, sizeof(buf));
	}
	fclose(fp);
	
	strcpy(bender, "not-found");
	return bender;
	
}





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






char* pgen_ip2str(const unsigned int addr){
	char *str;
	union lc{
		unsigned int l;
		unsigned char c[4];
	};
	union lc lc;
	lc.l = (int)addr;
	str = (char*)malloc(sizeof(char)*16);
	snprintf(str,sizeof(char[16]),"%u.%u.%u.%u",
					lc.c[0],lc.c[1],lc.c[2],lc.c[3]);
	return str;
}





char* pgen_mac2str(const u_char* addr){
	char *str;
	str = (char*)malloc(sizeof(char)*19);
	snprintf(str,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
		*addr, *(addr+1), *(addr+2), *(addr+3), *(addr+4), *(addr+5));
	return str;
}




void pgen_checkPrivileges(void){
	if(0 != getuid()){
		fprintf(stderr, "pgen : requirs root privileges\n");
		exit(-1);
	}
}


