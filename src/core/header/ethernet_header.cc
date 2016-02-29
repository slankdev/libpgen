



#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>
#include <arpa/inet.h>


namespace pgen {
 



struct eth {
    uint8_t dst[pgen::macaddress::length];
    uint8_t src[pgen::macaddress::length];
    uint16_t type;
};


void ethernet_header::write(void* buffer, size_t bufferlen) {
    if (bufferlen < min_length) {
        throw pgen::exception("pgen::ethernet_header::write: buflen is too small");
    }

    struct eth* p = (eth*)buffer;
    for (size_t i=0; i<pgen::macaddress::length; i++) {
        p->src[i] = src.get_octet(i+1);
        p->dst[i] = dst.get_octet(i+1);
    }
    p->type = htons(type);
}



void ethernet_header::read(const void* buffer, size_t buffer_len) {

    if (buffer_len < sizeof(eth)) {
        throw pgen::exception("pgen::ethernet_header::read: Buffer length is too small");
    }

    const eth* p = (const eth*)buffer;

    src.setbyarray(p->src);
    dst.setbyarray(p->dst);
    type = ntohs(p->type);
}


size_t ethernet_header::length() const {
    return ethernet_header::max_length;
}




} /* pgen */
