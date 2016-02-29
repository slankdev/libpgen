


#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>

#include <stdint.h>
#include <stddef.h>

#include <arpa/inet.h>



namespace pgen {




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
    if (buffer_len < (size_t)(this->hlen<<2)) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    struct ip* p = (ip*)buffer;
    p->version  = 4;
    p->hlen     = hlen;
    p->tos      = tos;
    p->id       = htons(id);
    p->frag_off = htons(frag_off);
    p->ttl      = ttl;
    p->protocol = protocol;
    p->check    = htons(check);
    memcpy(p->src, src.raw(), 4);
    memcpy(p->dst, dst.raw(), 4);

    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(p0, option, (size_t)(hlen<<2) - pgen::ipv4_header::min_length);
}



void ipv4_header::read(const void* buffer, size_t buffer_len) {
    if (buffer_len < pgen::ipv4_header::min_length) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    struct ip* p = (ip*)buffer;
    hlen     = p->hlen;
    tos      = p->tos;
    id       = ntohs(p->id);
    frag_off = ntohs(p->frag_off);
    ttl      = p->ttl;
    protocol = p->protocol;
    check    = ntohs(p->check);
    for (int i=0; i<4; i++) {
        src.set_octet(i+1, p->src[i]);
        dst.set_octet(i+1, p->dst[i]);
    }

    if (buffer_len < (size_t)(hlen<<2)) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(option, p0, (size_t)(hlen<<2) - pgen::ipv4_header::min_length);
}


size_t ipv4_header::length() const {
    return hlen << 2;
}




} /* namespace pgen */


