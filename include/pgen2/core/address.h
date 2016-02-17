
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {


class macaddress {
    public:
        static const size_t length = 6;
        static const size_t strlength = 3*length-1;
    private:

        uint8_t _raw[length];
        std::string _name;

        void _update_name();
        void  set_str(const std::string& str);
    public:
    
        macaddress();
        macaddress(const macaddress& m);
        macaddress(const std::string& str);
        macaddress(const char* str);
        void clear();

        const std::string get_str() const;
        const std::string get_bender() const;

        void set_octet(int index, uint8_t oct);
        uint8_t get_octet(int index) const;
        
        const uint8_t* raw() const; // I'v not used this function

        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        void setbydev(const char* ifname);
        void setbyarray(const uint8_t array[6]);
};




class ipaddress {
    public:
        static const size_t length4 = 4;
        static const size_t length6 = 16;
        static const size_t strlength4 = 3*length4-1;
        static const size_t strlength6 = 2*length6+7;
    private:

        uint8_t _raw4[4];
        uint16_t _raw6[8];
        std::string _name;
        bool _isV4; 

        void _update_name();
        void set_str(const std::string& str);
        
    public:

        ipaddress(bool isv4=true);
        ipaddress(const ipaddress& i);
        ipaddress(const std::string& str);
        ipaddress(const char* str);
        void clear();
        bool is_v4() const ;
        void set_version_v4(bool isv4);
        
        const std::string& get_str() const ;

        void set_octet4(int index, uint8_t oct);
        uint8_t get_octet4(int index) const ;
        void set_section6(int index, uint16_t num);
        uint16_t get_section6(int index) const ;

        const uint8_t* raw4() const ;
        const uint16_t* raw6() const ;

        ipaddress& operator=(const ipaddress& i);
        ipaddress& operator=(const std::string& str);
        ipaddress& operator=(const char* str);
        bool operator==(const ipaddress& addr) const ;
        bool operator!=(const ipaddress& addr) const ;

        // int setbydev(const char* ifname);
        // int setmaskbydev(const char* ifname);
        // int setnetbydev(const char* ifname);
        // int maskby(const ipaddress& mask);

};




} /* namespace pgen */


