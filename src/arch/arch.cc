






#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include <iostream> 
#include <exception>

#ifdef __PGEN_MAXOS
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>
#endif

#ifdef __PGEN_LINUX
#include <uapi/linux/if.h>
#endif


namespace pgen {
namespace arch {



void getmacbydev(const char* dev, uint8_t mac[6]) {
#ifdef __linux
    if(strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::arch::getmacbydev: Interface name size is too large");
    }

	int sockd;
	struct ifreq ifr;
	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        throw pgen::exception("pgen::arch::getmacbydev:socket: " + strerr(errno));
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, dev);
	if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0){
	    close(sockd);
        throw pgen::exception("pgen::arch::getmacbydev:ioctl: " + strerr(errno));
	}
	close(sockd);
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    
	return ; //success


#else // for bsd
    
	struct ifaddrs *ifap, *ifaptr;
    unsigned char *ptr;
	if(getifaddrs(&ifap) != 0){
        throw pgen::exception("pgen::arch::getmacbydev:getifaddrs: undefined error");
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
    throw pgen::exception("pgen::arch::getmacbydev: Unknown error");
}

        
} /* namespace arch */
} /* namespace pgen */




