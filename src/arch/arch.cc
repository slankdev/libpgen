






#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>
#include <pgen2/io/util.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <iostream> 
#include <exception>

#ifdef __PGEN_OSX
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>
#endif

#ifdef __PGEN_LINUX
#endif


namespace pgen {
namespace arch {


#if defined(__PGEN_LINUX)
int open_rawsock(const char* ifname) {
    throw pgen::exception("pgen::arch::open_rawsock() is not implemented yet");
    return 1;
}
#endif




#if defined(__PGEN_OSX)
int open_bpf(const char* ifname) {
    // throw pgen::exception("pgen::arch::open_bpf() is not implemented yet");
    // return 1;   
    int fd;
    struct ifreq ifr;
    const unsigned int one  = 1;

    int i;
    for (i = 0; i < 4; i++) { 
        std::string str = "/dev/bpf" + std::to_string(i);
        if ((fd = open(str.c_str(), O_RDWR)) > 0)
            break;
    }

    /* 
     * TODO 
     *  This if statement's condition may be vulnerable.
     *  Please fix condition, with relax.
     */
    if (i >= 5) { 
        std::string errmsg = "pgen::arch::open_bpf: cannot open BPF ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // set buffer size
    int bufsize = 4096;
    if (ioctl(fd, BIOCSBLEN, &bufsize) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: set buffer size ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // bind to device
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if(ioctl(fd, BIOCSETIF, &ifr) < 0){
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: bind to device ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // set promisc
    if (ioctl(fd, BIOCPROMISC, NULL) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: set promisc ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    //if recv packet then call read fast
    if (ioctl(fd, BIOCIMMEDIATE, &one) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // set recv sendPacket 
    if (ioctl(fd, BIOCSSEESENT, &one) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // flush recv buffer
    if (ioctl(fd, BIOCFLUSH, NULL) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: flush recv-buffer ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    // no complite src macaddr
    if (ioctl(fd, BIOCSHDRCMPLT, &one) < 0) {
        close(fd);
        std::string errmsg = "pgen::arch::open_bpf: no complite src-macaddress ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }

    return fd;
}
#endif




void getmacbydev(const char* dev, uint8_t mac[6]) {
#if defined(__PGEN_LINUX)
    if(strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::arch::getmacbydev: Interface name size is too large");
    }

    int sockd;
    struct ifreq ifr;
    if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        throw pgen::exception("pgen::arch::getmacbydev:socket: " + strerror(errno));
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, dev);
    if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0){
        close(sockd);
        throw pgen::exception("pgen::arch::getmacbydev:ioctl: " + strerror(errno));
    }
    close(sockd);
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    
    return ; //success


#elif defined(__PGEN_OSX)
    
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

 

void getipv4bydev(const char* dev, uint8_t ip[4]) {
    if(strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::arch::getipv4bydev: Interface name size is too large");
    }

    int sockd;
    struct ifreq ifr;
    struct sockaddr_in *sa;

    if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        std::string errmsg = "pgen::arch::getipv4bydev:socket: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, dev);
    if(ioctl(sockd, SIOCGIFADDR, &ifr) < 0){
        close(sockd);
        std::string errmsg = "pgen::arch::getipv4bydev:ioctl: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }
    close(sockd);
    sa = (struct sockaddr_in*)&ifr.ifr_addr;
    memcpy(ip, &(sa->sin_addr.s_addr), sizeof(uint32_t));
}


/* 
 * Thanks 
 *  http://d.hatena.ne.jp/shammer/20101212/p1 
 */
void getipv6bydev(const char* dev, uint16_t ip[8]) {
    if (strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::arch::getipv6bydev: Interface name size is too large");
    }

    struct ifaddrs * if_list;
    void * tmp;

    getifaddrs(&if_list);
    for (struct ifaddrs* ifa = if_list ; ifa != NULL ; ifa = ifa->ifa_next) {
        if (strcmp(ifa->ifa_name, dev) == 0) { 
            if (!ifa->ifa_addr) {
                continue;
            } else {
                if (ifa->ifa_addr->sa_family == AF_INET6) { 
                    tmp = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                    memcpy(ip, tmp, sizeof(struct in6_addr));
                    freeifaddrs(if_list);
                    return ;
                }
                else {
                    // For example, reach here the cases
                    // sa_family is AF_PACKET(17).
                    // sa_family is AF_INET6(23).
                }
            }
        }
    }
    freeifaddrs(if_list);
    throw pgen::exception("pgen::getipv6bydev: can't get address");
}


} /* namespace arch */
} /* namespace pgen */




