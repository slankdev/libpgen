


#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>

#include <stdint.h>
#include <stddef.h>




namespace pgen {


ip_header::ip_header() {
    hlen(5);
    tos(0);
    tot_len(0);
    id(0);
    frag_off(0);
    ttl(64);
    protocol(6);
    check(0);
    src("0.0.0.0");
    dst("0.0.0.0");
}


uint8_t ip_header::hlen() const {
    return _hlen;
}
uint8_t ip_header::tos() const {
    return _tos;
}
uint16_t   ip_header::tot_len() const {
    return _tot_len;
}
uint16_t   ip_header::id() const { 
    return _id;
}
uint16_t   ip_header::frag_off() const {
    return _frag_off;
}
uint8_t    ip_header::ttl() const {
    return _ttl;
}
uint8_t    ip_header::protocol() const {
    return _protocol;
}
uint16_t   ip_header::check() const {
    return _check;
}
const ipaddress&  ip_header::src() const {
    return _src;
}
const ipaddress&  ip_header::dst() const {
    return _dst;
}



void ip_header::hlen(uint8_t n) {
    struct {
        uint8_t bit:4;
    } r;
    _hlen = r.bit = n;
}
void ip_header::tos(uint8_t n) {
    _tos = n;
}
void ip_header::tot_len(uint16_t n) {
    printf("special!! \n");
    _tot_len = n;
}
void ip_header::id(uint16_t n) {
    _id = n;
}
void ip_header::frag_off(uint16_t n) {
    _frag_off = n;
}
void ip_header::ttl(uint8_t n) {
    _ttl = n;
}
void ip_header::protocol(uint8_t n) {
    _protocol = n;
}
void ip_header::check(uint16_t n) {
    _check = n;
}
void ip_header::src(const ipaddress& n) {
    _src = n;
}
void ip_header::dst(const ipaddress& n) {
    _dst = n;
}



void ip_header::write(void* buffer, size_t buffer_len) {
    if (buffer_len < pgen::ip_header::min_length4) {
        throw pgen::exception("pgen::ip_header::read: buflen is too small");
    }
    if (buffer == NULL) return ;
    return ;
}

size_t ip_header::read(const void* buffer, size_t buffer_len) {
    if (buffer_len < pgen::ip_header::min_length4) {
        throw pgen::exception("pgen::ip_header::read: buflen is too small");
    }
    if (buffer == NULL) return 1;
    return 1;
}

// const uint8_t* ip_header::raw() const {
//     return _raw;
// }

size_t ip_header::length() const {
    return 1;
}





} /* namespace pgen */


