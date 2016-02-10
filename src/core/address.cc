
#include <pgen2.h>
#include <pgen2/core/address.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <exception>


namespace pgen {
namespace core {
 

void macaddress::_update_name() {
    char str[18];
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", _raw[0], 
            _raw[1], _raw[2], _raw[3], _raw[4], _raw[5]);
    _name = str;
}


macaddress::macaddress() {
    clear();
}
macaddress::macaddress(const macaddress& m) {
    *this = m;
}
macaddress::macaddress(const std::string& str) {
    *this = str; 
}
macaddress::macaddress(const char* str) {
    std::string s = str;
    *this = s;
}


void macaddress::clear() {
    for (int i=0; i<6; i++)       
        _raw[i] = 0;
    _update_name();
}


void macaddress::set_str(const std::string& str) {
    if (str.length() != 17) {
        throw std::length_error("format error");
    }

    uint32_t buf[6];
	int n = sscanf(str.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x", 
			&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
    
    if (n != 6) {
        throw std::length_error("format error");
    }
	for(int i=0; i<6; i++)	_raw[i] = buf[i];

    _update_name();
}


const std::string macaddress::get_str() const {
    return _name;      
};





const std::string macaddress::get_bender() const {
    printf("macaddress::get_bender: This func was not implemented yet \n");
    return "";
}





void macaddress::set_octet(int index, uint8_t oct) {
    if (index < 1 || 6 < index) {
        throw std::out_of_range("index is faild");
    }
    _raw[index-1] = oct;
    _update_name();
}



uint8_t macaddress::get_octet(int index) const {
    if (index < 1 || 6 < index) {
        throw std::out_of_range("index is faild");
    }
    return _raw[index-1];
}


const uint8_t* macaddress::get_raw() const {
    return _raw;
}




macaddress& macaddress::operator=(const macaddress& m) {
    for (int i=0; i<6; i++) {
        _raw[i] = m._raw[i];      
    }
    _name = m._name;
    return *this;
}



macaddress& macaddress::operator=(const std::string& str) {
    set_str(str);
    return *this;
}


macaddress& macaddress::operator=(const char* str) {
    std::string s = str;
    *this = s;
    return *this;
}



bool macaddress::operator==(const macaddress& addr) const {
    for (int i=0; i<6; i++) {
        if (_raw[i] == addr.get_octet(i+1))
            continue;
        else
            return false;
    }
    return true;
}




bool macaddress::operator!=(const macaddress& addr) const {
    return !(*this==addr);
}



void macaddress::setmacbyarray(const uint8_t* array) {
    for (int i=0; i<6; i++) {  
        _raw[i] = array[i];      
    }
    _update_name();
}



void macaddress::setmacbydev(const char* ifname) {
    char buf[256];
    pgen::io::arch::getmacbydev(ifname, buf);
    *this = buf;
}




} /* namespace core */
} /* namespace pgen */
