


#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>

#include <stdint.h>
#include <stddef.h>

#include <arpa/inet.h>



namespace pgen {



uint8_t ipv4_header::hlen() const {
    return _hlen;
}
uint8_t ipv4_header::tos() const {
    return _tos;
}
uint16_t   ipv4_header::tot_len() const {
    return _tot_len;
}
uint16_t   ipv4_header::id() const { 
    return _id;
}
uint16_t   ipv4_header::frag_off() const {
    return _frag_off;
}
uint8_t    ipv4_header::ttl() const {
    return _ttl;
}
uint8_t    ipv4_header::protocol() const {
    return _protocol;
}
uint16_t   ipv4_header::check() const {
    return _check;
}
const ipv4address&  ipv4_header::src() const {
    return _src;
}
const ipv4address&  ipv4_header::dst() const {
    return _dst;
}



void ipv4_header::hlen(uint8_t n) {
    struct {
        uint8_t bit:4;
    } r;
    _hlen = r.bit = n;
}
void ipv4_header::tos(uint8_t n) {
    _tos = n;
}
void ipv4_header::tot_len(uint16_t n) {
    _tot_len = n;
}
void ipv4_header::id(uint16_t n) {
    _id = n;
}
void ipv4_header::frag_off(uint16_t n) {
    _frag_off = n;
}
void ipv4_header::ttl(uint8_t n) {
    _ttl = n;
}
void ipv4_header::protocol(uint8_t n) {
    _protocol = n;
}
void ipv4_header::check(uint16_t n) {
    _check = n;
}
void ipv4_header::src(const ipv4address& n) {
    _src = n;
}
void ipv4_header::dst(const ipv4address& n) {
    _dst = n;
}

const uint8_t* ipv4_header::option() const {
    return _option;
}
void ipv4_header::option(const void* buf, size_t buflen) {
    if (buflen > 40) {
        throw pgen::exception("pgen::ipv4_header::option: buflen is too large");
    }
    memcpy(_option, buf, buflen);
}



/*
 * TODO
 * __BYTE_ORDER macro is may imcomplete.
 * It is hard-coding on c language's header.
 * Redefine macro about OS's endian difference.
 */
struct ip {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t   hlen:4; 
    uint8_t   version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t   version:4;
    uint8_t   hlen:4; 
#else
# error	"Please fix endian.h"
#endif
    uint8_t   tos;
    uint16_t  tot_len; 
    uint16_t  id;
    uint16_t  frag_off;
    uint8_t   ttl;
    uint8_t   protocol;
    uint16_t  check;
    uint8_t   src[4];
    uint8_t   dst[4];     
};



void ipv4_header::write(void* buffer, size_t buffer_len) {
    if (buffer_len < (size_t)(this->hlen()<<2)) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    struct ip* p = (ip*)buffer;
    p->version  = 4;
    p->hlen     = hlen();
    p->tos      = tos();
    p->id       = htons(id());
    p->frag_off = htons(frag_off());
    p->ttl      = ttl();
    p->protocol = protocol();
    p->check    = htons(check());
    memcpy(p->src, _src.raw(), 4);
    memcpy(p->dst, _dst.raw(), 4);

    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(p0, _option, (size_t)(hlen()<<2) - pgen::ipv4_header::min_length);
}



void ipv4_header::read(const void* buffer, size_t buffer_len) {
    if (buffer_len < pgen::ipv4_header::min_length) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    struct ip* p = (ip*)buffer;
    hlen    (p->hlen);
    tos     (p->tos);
    id      (ntohs(p->id));
    frag_off(ntohs(p->frag_off));
    ttl     (p->ttl);
    protocol(p->protocol);
    check   (ntohs(p->check));
    for (int i=0; i<4; i++) {
        _src.set_octet(i+1, p->src[i]);
        _dst.set_octet(i+1, p->dst[i]);
    }

    if (buffer_len < (size_t)(hlen()<<2)) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(_option, p0, (size_t)(hlen()<<2) - pgen::ipv4_header::min_length);
}


size_t ipv4_header::length() const {
    return hlen() << 2;
}


// const uint8_t* ipv4_header::raw() const {
//     return _raw;
// }




} /* namespace pgen */


