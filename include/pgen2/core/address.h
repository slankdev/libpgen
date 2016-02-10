
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {
namespace core {


class macaddress {
    private:
        uint8_t _raw[6];
        std::string _name;

        void _update_name();
    public:
    
        macaddress();
        macaddress(const macaddress& m);
        macaddress(const std::string& str);
        macaddress(const char* str);
        void clear(void);

        void  set_str(const std::string& str);
        const std::string get_str() const ;
        const std::string get_bender() const ;

        void set_octet(int index, uint8_t oct);
        uint8_t get_octet(int index) const ;
        
        const uint8_t* get_raw() const;

        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        void setmacbyarray(const uint8_t* array);
        void setmacbydev(const char* ifname);
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




} /* namespace core */
} /* namespace pgen */
