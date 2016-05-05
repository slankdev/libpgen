

#include <pgen2/arch.h>
#include <pgen2/exception.h>
#include <pgen2/core/macaddress.h>
#include <pgen2/core/ipaddress.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <exception>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ifaddrs.h>


#ifdef PGEN_OSX
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>
#endif

#ifdef PGEN_LINUX
#include <unistd.h>
#endif

namespace pgen {




void getipv4bydev(const char* dev, uint8_t ip[4]) {
    if(strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::getipv4bydev: Interface name size is too large");
    }

    int sockd;
    struct ifreq ifr;

    if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        throw pgen::exception("pgen::getipv4bydev:socket: ");
    }
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, dev);
    int ret = ioctl(sockd, SIOCGIFADDR, &ifr);
    close(sockd);
    if(ret < 0){
        throw pgen::exception("pgen::getipv4bydev:ioctl: ");
    }
    struct sockaddr_in* sa = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
    memcpy(ip, &(sa->sin_addr.s_addr), sizeof(uint32_t));
}



ipv4address::ipv4address() {
    clear();
}


ipv4address::ipv4address(const ipv4address& i) {
    *this = i;
}

ipv4address::ipv4address(const std::string& str) {
    clear();
    set_str(str);
}

ipv4address::ipv4address(const char* str) {
    clear();
    set_str(str);
}

void ipv4address::clear() {
    for (size_t i=0; i<ipv4address::length; ++i)
        _raw[i] = 0;
    _update_name();
}


void ipv4address::_update_name() {
    _name.resize(strlength+1);
    std::fill(_name.begin(), _name.end(), 0);
    sprintf(&_name[0], "%d.%d.%d.%d", 
            _raw[0], _raw[1], _raw[2], _raw[3]);
    _name.resize(strlen(&_name[0]));
}

void ipv4address::set_str(const std::string& str) {
    int n = inet_pton(AF_INET, str.c_str(), _raw);
    if(n <= 0){
        throw pgen::exception("pgen::ipv4address::set_str(IPv4): Format error");
    }
    _update_name();
}


void ipv4address::set_octet(int index, uint8_t oct) {
    if (index < 1 || 4 < index) {
        throw pgen::exception("pgen::ipv4address::set_octet4: index, 1st-arg is faild");
    }
    _raw[index-1] = oct;
    _update_name();
}

uint8_t ipv4address::get_octet(int index) const  {
    if (index < 1 || 4 < index) {
        throw pgen::exception("pgen::ipv4address::set_octet: index, 1st-arg is faild");
    }
    return _raw[index-1];
}

const void* ipv4address::raw() const {
    return _raw;
}


const std::string& ipv4address::str() const  {
    return _name;
}


ipv4address& ipv4address::operator=(const ipv4address& rhs) {
    memcpy(_raw, rhs._raw, ipv4address::length);
    _name = rhs._name;
    return *this;
}

ipv4address& ipv4address::operator=(const std::string& rhs) {
    set_str(rhs);
    return *this;
}

ipv4address& ipv4address::operator=(const char* rhs) {
    set_str(rhs);
    return *this;
}

bool ipv4address::operator==(const ipv4address& rhs) const {
    return memcmp(_raw, rhs._raw, ipv4address::length) == 0;
}

bool ipv4address::operator!=(const ipv4address& rhs) const {
    return !(*this==rhs);
}

void ipv4address::setbydev(const char* ifname) {
    pgen::getipv4bydev(ifname, _raw);
    _update_name();
}


void ipv4address::setbyarray(const uint8_t array[4]) {
    memcpy(_raw, array, length);
    _update_name();
}


void ipv4address::copytoarray(uint8_t array[4]) const {
    memcpy(array, _raw, length);
}







/* 
 * Thanks 
 *  http://d.hatena.ne.jp/shammer/20101212/p1 
 */
void getipv6bydev(const char* dev, uint16_t ip[8]) {
    if (strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::getipv6bydev: Interface name size is too large");
    }

    struct ifaddrs * if_list;
    void * tmp;

    getifaddrs(&if_list);
    for (struct ifaddrs* ifa = if_list ; ifa != nullptr ; ifa = ifa->ifa_next) {
        if (strcmp(ifa->ifa_name, dev) == 0) { 
            if (!ifa->ifa_addr) {
                continue;
            } else {
                if (ifa->ifa_addr->sa_family == AF_INET6) { 

                    // TODO this is complex casting code
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








ipv6address::ipv6address() {
    clear();
}



ipv6address::ipv6address(const ipv6address& i) {
    *this = i;
}

ipv6address::ipv6address(const std::string& str) {
    clear();
    set_str(str);
}

ipv6address::ipv6address(const char* str) {
    clear();
    set_str(str);
}

void ipv6address::clear() {
    for (size_t i=0; i<ipv6address::length; ++i)
        _raw[i] = 0;
    _update_name();
}




void ipv6address::_update_name() {
    _name.resize(strlength+1);
    std::fill(_name.begin(), _name.end(), 0);
    sprintf(&_name[0], "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
            ntohs(_raw[0]), ntohs(_raw[1]), ntohs(_raw[2]), ntohs(_raw[3]),
            ntohs(_raw[4]), ntohs(_raw[5]), ntohs(_raw[6]), ntohs(_raw[7]));
    _name.resize(strlen(&_name[0]));
}

void ipv6address::set_str(const std::string& str) {
    int n = inet_pton(AF_INET6, str.c_str(), _raw);
    if(n == 0){
        throw pgen::exception("pgen::ipv6address::set_str(IPv6): Format error");
    }
    _update_name();
}


void ipv6address::set_section(int index, uint16_t num) {
    if (index < 1 || 8 < index) {
        throw pgen::exception("pgen::ipv6address::set_section6: index, 1st-arg is faild");
    }
    _raw[index-1] = num;
    _update_name();
}

uint16_t ipv6address::get_section(int index) const  {
    if (index < 1 || 8 < index) {
        throw pgen::exception("pgen::ipv6address::get_section6: index, 1st-arg is faild");
    }
    return _raw[index-1];
}

const void* ipv6address::raw() const {
    return _raw;
}



const std::string& ipv6address::str() const  {
    return _name;
}


ipv6address& ipv6address::operator=(const ipv6address& rhs) {
    memcpy(_raw, rhs._raw, ipv6address::length);
    _name = rhs._name;
    return *this;
}

ipv6address& ipv6address::operator=(const std::string& rhs) {
    set_str(rhs);
    return *this;
}

ipv6address& ipv6address::operator=(const char* rhs) {
    set_str(rhs);
    return *this;
}

bool ipv6address::operator==(const ipv6address& rhs) const {
    return memcmp(_raw, rhs._raw, ipv6address::length) == 0;
}

bool ipv6address::operator!=(const ipv6address& rhs) const {
    return !(*this==rhs);
}

void ipv6address::setbydev(const char* ifname) {
    pgen::getipv6bydev(ifname, _raw);
    _update_name();
}

void ipv6address::setbyarray(const uint16_t array[8]) {
    memcpy(_raw, array, length);
    _update_name();
}

void ipv6address::copytoarray(uint16_t array[8]) const {
    memcpy(array, _raw, length);
}

} /* namespace pgen */
