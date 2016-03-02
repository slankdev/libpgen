

#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>
#include <pgen2/core/macaddress.h>
#include <pgen2/core/ipaddress.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <exception>
#include <arpa/inet.h>



namespace pgen {





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
    pgen::arch::getipv4bydev(ifname, _raw);
    _update_name();
}


void ipv4address::setbyarray(const uint8_t array[4]) {
    memcpy(_raw, array, length);
    _update_name();
}


void ipv4address::copytoarray(uint8_t array[4]) const {
    memcpy(array, _raw, length);
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
    pgen::arch::getipv6bydev(ifname, _raw);
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
