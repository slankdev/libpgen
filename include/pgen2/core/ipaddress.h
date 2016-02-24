
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {




class ipv4address {
    public:

        static const size_t length = 4;
        static const size_t strlength = 3*length+3;

    private:

        uint8_t _raw[length];
        std::string _name;

        void _update_name();
        void set_str(const std::string& str);

    public:

        ipv4address();
        ipv4address(const ipv4address& i);
        ipv4address(const std::string& str);
        ipv4address(const char* str);
        void clear();
        
        const void* raw() const ;
        const std::string& str() const ;

        void     set_octet(int index, uint8_t oct);
        uint8_t  get_octet(int index) const ;

        ipv4address& operator=(const ipv4address& i);
        ipv4address& operator=(const std::string& str);
        ipv4address& operator=(const char* str);
        bool operator==(const ipv4address& addr) const ;
        bool operator!=(const ipv4address& addr) const ;

        void setbydev(const char* ifname);

        // uint8_t& operator [] (int index); // for v4
        // int setmaskbydev(const char* ifname);
        // int setnetbydev(const char* ifname);
        // int maskby(const ipaddress& mask);
};


class ipv6address {
    public:

        static const size_t length = 16;
        static const size_t strlength = 2*length+7;

    private:

        uint16_t _raw[length / sizeof(uint16_t)];
        std::string _name;

        void _update_name();
        void set_str(const std::string& str);

    public:

        ipv6address();
        ipv6address(const ipv6address& i);
        ipv6address(const std::string& str);
        ipv6address(const char* str);
        void clear();
        
        const void* raw() const ;
        const std::string& str() const ;

        void     set_section(int index, uint16_t num);
        uint16_t get_section(int index) const ;

        ipv6address& operator=(const ipv6address& i);
        ipv6address& operator=(const std::string& str);
        ipv6address& operator=(const char* str);
        bool operator==(const ipv6address& addr) const ;
        bool operator!=(const ipv6address& addr) const ;

        void setbydev(const char* ifname);

        // uint8_t& operator [] (int index); // for v4
        // int setmaskbydev(const char* ifname);
        // int setnetbydev(const char* ifname);
        // int maskby(const ipaddress& mask);
};




} /* namespace pgen */

