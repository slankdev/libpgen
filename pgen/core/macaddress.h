
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {


void getmacbydev(const char* dev, uint8_t mac[6]);

/**
 * This class provides MAC address functionality.
 **/
class macaddress {
    public:
        static const size_t length = 6;             /**< Address length         */
        static const size_t strlength = 3*length-1; /**< Address't c_str length */
    private:

        uint8_t _raw[length];
        std::string _name;

        void _update_name();
        void  set_str(const std::string& str);

    public:

        macaddress();                         /**< default constructor                */
        macaddress(const macaddress& m);      /**< copy constructor                   */
        macaddress(const std::string& str);   /**< init address from std::string&     */
        macaddress(const char* str);          /**< init address from const char*      */
        void clear();                         /**< clear address "0.0.0.0"            */

        const uint8_t* raw() const;           /**< return address raw data            */
        const std::string str() const;        /**< get address string as std::string& */
        const std::string get_bender() const; /**< bender string, NOT IMPLEMENT       */

        /**
         * Set octet data.
         * @param[in] index address index. (1-6)
         * @param[in] oct octet data (0-255)
         **/
        void set_octet(int index, uint8_t oct);

        /**
         * Get octet data.
         * @param[in] index address index. (1-6)
         * @return octet data
         **/
        uint8_t get_octet(int index) const;


        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        /**
         * Set address by device name.
         * @param[in] ifname interface name. (ex, "eth0", "wlan0")
         **/
        void setbydev(const char* ifname);

        /**
         * Set address by array.
         * @param[in] array address array. (uint8_t[6])
         **/
        void setbyarray(const uint8_t array[6]);

        void copytoarray(uint8_t array[6]) const;

        void setbcast();
};


} /* namespace pgen */











/*
 * Func IMple
 */



#include <pgen/arch.h>
#include <pgen/exception.h>
#include <pgen/core/macaddress.h>
#include <pgen/core/ipaddress.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <exception>
#include <arpa/inet.h>

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



inline void getmacbydev(const char* dev, uint8_t mac[6]) {
#if defined(PGEN_LINUX)
    if(strlen(dev) >= IFNAMSIZ) {
        throw pgen::exception("pgen::getmacbydev: Interface name size is too large");
    }

    int sockd;
    struct ifreq ifr;
    if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        throw pgen::exception("pgen::getmacbydev:socket: ");
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, dev);

    int ret = ioctl(sockd, SIOCGIFHWADDR, &ifr);
    close(sockd);
    if (ret < 0) {
        throw pgen::exception("pgen::getmacbydev:ioctl: ");
    }
    memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);

    return ; //success


#elif defined(PGEN_OSX)

    struct ifaddrs *ifap, *ifaptr;
    unsigned char *ptr;
    if(getifaddrs(&ifap) != 0){
        throw pgen::exception("pgen::getmacbydev:getifaddrs: undefined error");
    }

    for(ifaptr = ifap; ifaptr != nullptr; ifaptr = (ifaptr)->ifa_next) {
        if (!strcmp((ifaptr)->ifa_name, dev) &&
                (((ifaptr)->ifa_addr)->sa_family == AF_LINK)) {
            ptr = (unsigned char *)LLADDR((struct sockaddr_dl *)(ifaptr)->ifa_addr);
            memcpy(mac, ptr, 6);
            freeifaddrs(ifap);
            return; // success
        }
    }


#endif
    throw pgen::exception("pgen::getmacbydev: Unknown error");
}








inline macaddress::macaddress() {
    clear();
}

inline macaddress::macaddress(const macaddress& m) {
    *this = m;
}

inline macaddress::macaddress(const std::string& str) {
    set_str(str);
}

inline macaddress::macaddress(const char* str) {
    set_str(str);
}

inline void macaddress::clear(){
    memset(_raw, 0, sizeof(_raw));
    _update_name();
}



inline void macaddress::_update_name() {
    _name.resize(strlength+1);
    sprintf(&_name[0], "%02x:%02x:%02x:%02x:%02x:%02x", _raw[0],
            _raw[1], _raw[2], _raw[3], _raw[4], _raw[5]);
    _name.resize(strlength);
}

inline void macaddress::set_str(const std::string& str) {
    if (str.length() != strlength) {
        throw pgen::exception("pgen::macaddress::set_str: Format error");
    }

    uint32_t buf[length];
    int n = sscanf(str.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
            &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);

    if (n != (int)length) {
        throw pgen::exception("pgen::macaddress::set_str: Format error");
    }
    for(size_t i=0; i<length; ++i)  _raw[i] = buf[i];

    _update_name();
}

inline const std::string macaddress::str() const {
    return _name;
};

inline const std::string macaddress::get_bender() const {
    throw pgen::exception("pgen::macaddress::get_bender: Not implement yet");
}


inline void macaddress::set_octet(int index, uint8_t oct) {
    if (index < 1 || 6 < index) {
        throw pgen::exception("pgen::macaddress::set_octet: index, 1st-arg is faild");
    }
    _raw[index-1] = oct;
    _update_name();
}

inline uint8_t macaddress::get_octet(int index) const {
    if (index < 1 || 6 < index) {
        throw pgen::exception("pgen::macaddress::set_octet: index, 1st-arg is faild");
    }
    return _raw[index-1];
}

inline const uint8_t* macaddress::raw() const {
    return _raw;
}

inline macaddress& macaddress::operator=(const macaddress& rhs) {
    memcpy(_raw, rhs._raw, sizeof(_raw));
    _name = rhs._name;
    return *this;
}

inline macaddress& macaddress::operator=(const std::string& str) {
    set_str(str);
    return *this;
}

inline macaddress& macaddress::operator=(const char* str) {
    set_str(str);
    return *this;
}

inline bool macaddress::operator==(const macaddress& rhs) const {
    return memcmp(_raw, rhs._raw, sizeof(_raw)) == 0;
}

inline bool macaddress::operator!=(const macaddress& rhs) const {
    return !(*this==rhs);
}

inline void macaddress::setbydev(const char* ifname) {
    pgen::getmacbydev(ifname, _raw);
    _update_name();
}

inline void macaddress::setbyarray(const uint8_t array[6]) {
    memcpy(_raw, array, length);
    _update_name();
}

inline void macaddress::copytoarray(uint8_t array[6]) const {
    memcpy(array, _raw, length);
}


inline void macaddress::setbcast() {
    *this = "ff:ff:ff:ff:ff:ff";
}

} /* namespace pgen */
