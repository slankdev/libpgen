

#include "util.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if_dl.h>

// #include <net/bpf.h>
// #include <fcntl.h>
// #include <ifaddrs.h>

#include <iostream> 
#include <exception>

namespace pgen {
namespace io {
namespace arch {



void getmacbydev(const char* dev, char* mac) {
#ifdef __linux

	int sockd;
	struct ifreq ifr;
	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        throw std::domain_error(strerr(errno));
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);
	if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0){
        throw std::domain_error(strerr(errno));
	}
	close(sockd);
	sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x", 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[0], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[1], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[2], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[3], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[4], 
	 	(unsigned char)ifr.ifr_hwaddr.sa_data[5] );
	return ; //success

#else // for bsd
    
	struct ifaddrs *ifap, *ifaptr;
    unsigned char *ptr;
	if(getifaddrs(&ifap) != 0){
        throw std::domain_error("undefined");
	}
	for(ifaptr = ifap; ifaptr != NULL; ifaptr = (ifaptr)->ifa_next) {
		if (!strcmp((ifaptr)->ifa_name, dev) && 
				(((ifaptr)->ifa_addr)->sa_family == AF_LINK)) {
			ptr = (unsigned char *)LLADDR((struct sockaddr_dl *)(ifaptr)->ifa_addr);
			sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
								*ptr, *(ptr+1), *(ptr+2), 
								*(ptr+3), *(ptr+4), *(ptr+5));
			freeifaddrs(ifap);
			return; // success
		}
	}

#endif
}

        
} /* namespace arch */
} /* namespace io */
} /* namespace pgen */




