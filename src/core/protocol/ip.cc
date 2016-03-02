

#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/protocol/ip.h>
#include <pgen2/exception.h>



namespace pgen {
    



/*
 * TODO about struct ip
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



void ipv4_header::clear() {
        hlen = 5; 
        tos = 0x0;
        tot_len = 20; 
        id = 0x0;
        frag_off = 0x0;
        ttl = 64;
        protocol = 0;
        check = 0;
        src = "0.0.0.0";
        dst = "0.0.0.0";     
        memset(option, 0, sizeof(option));
}



void ipv4_header::summary(bool moreinfo) const {
    printf("IP [%s -> %s protocol=%d] \n",
            src.str().c_str(), dst.str().c_str(), protocol);

    if (moreinfo) {
        printf(" - header len     : %d \n", (int)hlen    );
        printf(" - type of service: %d \n", (int)tos     );
        printf(" - total length   : %d \n", (int)tot_len );
        printf(" - identifer      : %d \n", (int)id      );
        printf(" - fragment offset: %d \n", (int)frag_off);
        printf(" - time to leave  : %d \n", (int)ttl     );
        printf(" - protocol       : %d \n", (int)protocol);
        printf(" - checksum       : %d \n", (int)check   );
        printf(" - source         : %s \n", src.str().c_str());
        printf(" - destination    : %s \n", dst.str().c_str());
    }
}


void ipv4_header::write(void* buffer, size_t buffer_len) const {
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

    // TODO implement src.copyto(p->src);
    // TODO implement dst.copyto(p->dst);
    memcpy(p->src, src.raw(), 4);
    memcpy(p->dst, dst.raw(), 4);
    
    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(p0, option, length() - pgen::ipv4_header::min_length);
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

    // TODO reimplement src.setbyarray(p->src);
    // TODO reimplement dst.setbyarray(p->dst);
    for (int i=0; i<4; i++) {
        src.set_octet(i+1, p->src[i]);
        dst.set_octet(i+1, p->dst[i]);
    }

    if (buffer_len < size_t(hlen)*4) {
        throw pgen::exception("pgen::ipv4_header::read: buflen is too small");
    }

    uint8_t* p0 = (uint8_t*)p + pgen::ipv4_header::min_length;
    memcpy(option, p0, length() - pgen::ipv4_header::min_length);
}



size_t ipv4_header::length() const {
    return hlen * 4;
}




ipv4::ipv4() {
    clear();
    init_headers();
}


ipv4::ipv4(const void* buffer, size_t bufferlen) : ipv4() {
    analyze(buffer, bufferlen);
}


ipv4::ipv4(const pgen::ipv4& rhs) : packet(rhs) {
    ETH = rhs.ETH;
    IP  = rhs.IP;
    init_headers();
}



void ipv4::clear() {
    ETH.clear();
    ETH.type = 0x0800;
    IP.clear();
}


void ipv4::init_headers() {
    headers = {&ETH, &IP};
}




} /* namespace pgen */
