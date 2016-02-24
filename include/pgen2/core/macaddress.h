
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
        
        const uint8_t* raw() const; 

        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        void setbydev(const char* ifname);
        void setbyarray(const uint8_t array[6]);
};





} /* namespace pgen */
