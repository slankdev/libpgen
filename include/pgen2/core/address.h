
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {


class macaddress {
    private:
        static const size_t _length = 6;
        static const size_t strlength = 3*_length - 1;

        uint8_t _raw[_length];
        std::string _name;

        void _update_name();
        void  set_str(const std::string& str);
    public:
    
        macaddress();
        macaddress(const macaddress& m);
        macaddress(const std::string& str);
        macaddress(const char* str);
        void clear();
        size_t length() const;

        const std::string get_str() const;
        const std::string get_bender() const;

        void set_octet(int index, uint8_t oct);
        uint8_t get_octet(int index) const;
        
        const uint8_t* get_raw() const;

        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        void setbydev(const char* ifname);
        void setbyarray(const uint8_t array[6]);
};




class ipaddress {
    private:
        uint32_t _rawv4;
        uint8_t _rawv6[16];
        std::string _name;
        bool _isV4; 
        
    public:

        ipaddress(bool isv4=true);
        ipaddress(const ipaddress& i);
        ipaddress(const std::string& str);
        ipaddress(const char* str);
        void clear();
        
        void set_str(const std::string& str);
        const std::string& get_str() const ;

        void setOctet(int index, uint8_t oct);
        uint8_t getOctet(int index) const ;
        void setRaw4(uint32_t raw); 
        uint32_t getRaw4() const ;
        void setRaw6(uint32_t raw);  // mitei
        uint32_t getRaw6() const ;   // mitei

        int setipbydev(const char* ifname);
        int setmaskbydev(const char* ifname);
        int setnetbydev(const char* ifname);
        int maskby(const ipaddress& mask);

        ipaddress& operator=(const ipaddress& i);
        ipaddress& operator=(const std::string& str);
        ipaddress& operator=(const char* str);
        bool operator==(const ipaddress& addr) const ;
        bool operator!=(const ipaddress& addr) const ;
};




} /* namespace pgen */


