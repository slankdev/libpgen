

#include <stdint.h>
#include <stddef.h>
#include <pgen2/core/protocol/ipv6.h>
#include <pgen2/exception.h>
#include <arpa/inet.h>


namespace pgen {
    
/**
 * TODO fix struct ipv6_struct 
 **/
struct ipv6_struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t trafic_class1:4;
    uint8_t version:4;
    uint8_t flow_level1:4;
    uint8_t trafic_class2:4;
    uint16_t flow_level2:16;
#elif __BYTE_ORDER == __BIG_ENDIAN
# error	"not implement yet"
#else 
# error	"Please fix endian.h"
#endif
    uint16_t payload_len;
    uint8_t next_header;
    uint8_t hop_limit;
    uint16_t src[8];
    uint16_t dst[8];
};




void ipv6_header::clear() {
    version      = 6;
    trafic_class = 0;
    flow_level   = 0;
    payload_len  = 0;
    next_header  = 6;
    hop_limit    = 64;
    src.clear();
    dst.clear();
}



void ipv6_header::summary(bool moreinfo) const {
    printf("IPv6 [%s -> %s]\n", src.str().c_str(), dst.str().c_str());

    if (moreinfo) {
        printf(" - version        : %d \n",     int(version)     );
        printf(" - trafic code    : 0x%02x \n", trafic_class     );
        printf(" - flow level     : 0x%06x \n", flow_level       );
        printf(" - payload length : %d \n",     int(payload_len) );
        printf(" - next header    : %d \n",     int(next_header) );
        printf(" - hop limit      : %d \n",     int(hop_limit)   );
        printf(" - source         : %s \n",     src.str().c_str());
        printf(" - destination    : %s \n",     dst.str().c_str());
    }
}



void ipv6_header::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < min_length) {
        throw pgen::exception(
                "pgen::ipv6_header::write: bufferlen is too small");
    }

    
    struct ipv6_struct* p = reinterpret_cast<ipv6_struct*>(buffer);
    p->version = version;
    p->trafic_class1 = 0;
    p->trafic_class2 = 0;
    p->flow_level1   = 0;
    p->flow_level2   = 0;
    p->payload_len   = htons(payload_len);
    p->next_header   = next_header;
    p->hop_limit     = hop_limit;
    src.copytoarray(p->src);
    dst.copytoarray(p->dst);
}



void ipv6_header::read(const void* buffer, size_t bufferlen) { 
    if (bufferlen < min_length) {
        throw pgen::exception(
                "pgen::ipv6_header::read: bufferlen is too small");
    }

    const struct ipv6_struct* p = reinterpret_cast<const ipv6_struct*>(buffer);
    version      = p->version;
    trafic_class = 0;
    flow_level   = 0;
    payload_len  = ntohs(p->payload_len);
    next_header  = p->next_header;
    hop_limit    = p->hop_limit;
    src.setbyarray(p->src);
    dst.setbyarray(p->dst);
}


size_t ipv6_header::length() const {
    return min_length; 
}







ipv6::ipv6::ipv6() {
    clear();
    printf("test \n");
    init_headers();
}

ipv6::ipv6(const void* buffer, size_t bufferlen) : ipv6() {
    analyze(buffer, bufferlen);
}

ipv6::ipv6(const pgen::ipv6& rhs) : packet(rhs) {
    ETH = rhs.ETH;
    IP  = rhs.IP;
    init_headers();
}




void ipv6::clear() {
    ETH.clear();
    ETH.type = pgen::ethernet::type::ipv6;
    IP.clear();
}


void ipv6::init_headers() {
    headers = {&ETH, &IP};
}



} /* namespace pgen */
