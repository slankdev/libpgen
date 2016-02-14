

#include <pgen2/io/util.h>

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



void getmacbydev(const char* dev, uint8_t mac[6]) {
#ifdef __linux
    if(strlen(dev) >= IFNAMSIZ) {
        // throw  
        printf("atode \n");
        exit(-1);
    }

	int sockd;
	struct ifreq ifr;
	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        throw std::domain_error(strerr(errno));
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, dev);
	if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0){
	    close(sockd);
        throw std::domain_error(strerr(errno));
	}
	close(sockd);
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    
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
            memcpy(mac, ptr, 6);
			freeifaddrs(ifap);
			return; // success
		}
	}
    

#endif
    throw std::domain_error("not support");
}

        
} /* namespace arch */
} /* namespace io */
} /* namespace pgen */




