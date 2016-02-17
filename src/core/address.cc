#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>
#include <pgen2/core/address.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <exception>
#include <arpa/inet.h>
#include <string.h>


namespace pgen {
 
macaddress::macaddress() {
    clear();
}
macaddress::macaddress(const macaddress& m) {
    *this = m;
}
macaddress::macaddress(const std::string& str) {
    set_str(str);
}
macaddress::macaddress(const char* str) {
    set_str(str);
}
void macaddress::clear(){
    memset(_raw, 0, sizeof(_raw));
    _update_name();
}



void macaddress::_update_name() {
    _name.resize(strlength+1);
    sprintf(&_name[0], "%02x:%02x:%02x:%02x:%02x:%02x", _raw[0],
            _raw[1], _raw[2], _raw[3], _raw[4], _raw[5]);
    _name.resize(strlength);
}

void macaddress::set_str(const std::string& str) {
    if (str.length() != strlength) {
        throw pgen::exception("pgen::macaddress::set_str: Format error");
    }

    uint32_t buf[length];
    int n = sscanf(str.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
            &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
    
    if (n != (int)length) {
        throw pgen::exception("pgen::macaddress::set_str: Format error");
    }
    for(size_t i=0; i<length; i++)  _raw[i] = buf[i];

    _update_name();
}

const std::string macaddress::get_str() const {
    return _name;      
};

const std::string macaddress::get_bender() const {
    throw pgen::exception("pgen::macaddress::get_bender: Not implement yet");
}


void macaddress::set_octet(int index, uint8_t oct) {
    if (index < 1 || 6 < index) {
        throw pgen::exception("pgen::macaddress::set_octet: index, 1st-arg is faild");
    }
    _raw[index-1] = oct;
    _update_name();
}

uint8_t macaddress::get_octet(int index) const {
    if (index < 1 || 6 < index) {
        throw pgen::exception("pgen::macaddress::set_octet: index, 1st-arg is faild");
    }
    return _raw[index-1];
}

const uint8_t* macaddress::raw() const {
    return _raw;
}

macaddress& macaddress::operator=(const macaddress& rhs) {
    memcpy(_raw, rhs._raw, sizeof(_raw));
    _name = rhs._name;
    return *this;
}

macaddress& macaddress::operator=(const std::string& str) {
    set_str(str);
    return *this;
}

macaddress& macaddress::operator=(const char* str) {
    set_str(str);
    return *this;
}

bool macaddress::operator==(const macaddress& rhs) const {
    return memcmp(_raw, rhs._raw, sizeof(_raw)) == 0;
}

bool macaddress::operator!=(const macaddress& rhs) const {
    return !(*this==rhs);
}

void macaddress::setbydev(const char* ifname) {
    pgen::arch::getmacbydev(ifname, _raw);
    _update_name();
}

void macaddress::setbyarray(const uint8_t array[6]) {
    memcpy(_raw, array, length);
    _update_name();
}






/*
 * IP address class
 *
 *
 */
ipaddress::ipaddress(bool isv4) {
    _isV4 = isv4;
    clear();
}

ipaddress::ipaddress(const ipaddress& i) {
    *this = i;
}

ipaddress::ipaddress(const std::string& str) : _isV4(true) {
    clear();
    set_str(str);
}

ipaddress::ipaddress(const char* str) : _isV4(true) {
    clear();
    set_str(str);
}

void ipaddress::clear() {
    for (size_t i=0; i<ipaddress::length4; i++)
        _raw4[i] = 0;
    for (size_t i=0; i<ipaddress::length6; i++)
        _raw6[i] = 0;
    _update_name();
}


bool ipaddress::is_v4() const {
    return _isV4;
}
void ipaddress::set_version_v4(bool isv4) {
    _isV4 = isv4;
    _update_name();
}


void ipaddress::_update_name() {
    if (_isV4) {
        _name.resize(strlength4+1);
        std::fill(_name.begin(), _name.end(), 0);
        sprintf(&_name[0], "%d.%d.%d.%d", 
                _raw4[0], _raw4[1], _raw4[2], _raw4[3]);
        _name.resize(strlen(&_name[0]));
    } else {
        _name.resize(strlength6+1);
        std::fill(_name.begin(), _name.end(), 0);
        sprintf(&_name[0], "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
                ntohs(_raw6[0]), ntohs(_raw6[1]), ntohs(_raw6[2]), ntohs(_raw6[3]),
                ntohs(_raw6[4]), ntohs(_raw6[5]), ntohs(_raw6[6]), ntohs(_raw6[7]));
        _name.resize(strlen(&_name[0]));
    }
}

void ipaddress::set_str(const std::string& str) {
    if (_isV4) {
        int n = inet_pton(AF_INET, str.c_str(), _raw4);
        if(n == 0){
            throw pgen::exception("pgen::ipaddress::set_str(IPv4): Format error");
        }else if(n == -1){
            std::string error_str = "pgen::ipaddress::set_str(IPv4): Format error ";
            error_str += strerror(errno);
            throw pgen::exception(error_str);
        }
    } else {
        int n = inet_pton(AF_INET6, str.c_str(), _raw6);
        if(n == 0){
            throw pgen::exception("pgen::ipaddress::set_str(IPv6): Format error");
        }else if(n == -1){
            std::string error_str = "pgen::ipaddress::set_str(IPv6): Format error ";
            error_str += strerror(errno);
            throw pgen::exception(error_str);
        }
    }
    _update_name();
}


void ipaddress::set_octet4(int index, uint8_t oct) {
    if (_isV4) {
        if (index < 1 || 4 < index) {
            throw pgen::exception("pgen::ipaddress::set_octet4: index, 1st-arg is faild");
        }
        _raw4[index-1] = oct;
        _update_name();
    } else {
        throw pgen::exception("pgen::ipaddress::set_octet4: ipv6 address can't use octet");
    }
}

uint8_t ipaddress::get_octet4(int index) const  {
    if (_isV4) { 
        if (index < 1 || 4 < index) {
            throw pgen::exception("pgen::ipaddress::set_octet: index, 1st-arg is faild");
        }
        return _raw4[index-1];
    } else {
        throw pgen::exception("pgen::ipaddress::set_octet4: ipv6 address can't use octet");
    }
}

void ipaddress::set_section6(int index, uint16_t num) {
    if (_isV4) {
        throw pgen::exception("pgen::ipaddress::set_section6: ipv4 address can't use section");
    } else {
        if (index < 1 || 8 < index) {
            throw pgen::exception("pgen::ipaddress::set_section6: index, 1st-arg is faild");
        }
        _raw6[index-1] = num;
        _update_name();
    }
}

uint16_t ipaddress::get_section6(int index) const  {
    if (_isV4) {
        throw pgen::exception("pgen::ipaddress::get_section6: ipv4 address can't use section");
    } else {
        if (index < 1 || 8 < index) {
            throw pgen::exception("pgen::ipaddress::get_section6: index, 1st-arg is faild");
        }
        return _raw6[index-1];
    }
}

const void* ipaddress::raw() const {
    if (_isV4) 
        return _raw4;
    else 
        return _raw6;
}


size_t ipaddress::length() const {
    if (_isV4) 
        return length4;
    else 
        return length6;
}

const std::string& ipaddress::get_str() const  {
    return _name;
}


ipaddress& ipaddress::operator=(const ipaddress& rhs) {
    _isV4 = rhs._isV4;
    if (_isV4)
        memcpy(_raw4, rhs._raw4, ipaddress::length4);
    else 
        memcpy(_raw6, rhs._raw6, ipaddress::length6);
    _name = rhs._name;
    return *this;
}

ipaddress& ipaddress::operator=(const std::string& rhs) {
    set_str(rhs);
    return *this;
}

ipaddress& ipaddress::operator=(const char* rhs) {
    set_str(rhs);
    return *this;
}

bool ipaddress::operator==(const ipaddress& rhs) const {
    if (_isV4)
        return memcmp(_raw4, rhs._raw4, ipaddress::length4) == 0;
    else
        return memcmp(_raw6, rhs._raw6, ipaddress::length6) == 0;
}

bool ipaddress::operator!=(const ipaddress& rhs) const {
    return !(*this==rhs);
}

void ipaddress::setbydev(const char* ifname) {
    if (_isV4) {
        pgen::arch::getipv4bydev(ifname, _raw4);
        _update_name();
    } else {
        pgen::arch::getipv6bydev(ifname, _raw6);
        _update_name();
    }
}


} /* namespace pgen */
