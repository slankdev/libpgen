

#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/exception.h>

namespace pgen {




struct eth {
    uint8_t dst[pgen::macaddress::length];
    uint8_t src[pgen::macaddress::length];
    uint16_t type;
};



void ethernet_header::clear() {
    src  = "00:00:00:00:00:00";
    dst  = "00:00:00:00:00:00";
    type = 0x0000;
}


void ethernet_header::summary(bool moreinfo) const {
    printf("Ethernet [%s -> %s type=0x%04x] \n", 
            src.str().c_str(),
            dst.str().c_str(), type);

    if (moreinfo) {
        printf(" - source      : %s \n", src.str().c_str());
        printf(" - destination : %s \n", dst.str().c_str());
        printf(" - type        : 0x%04x \n", type);
    } 
}


void ethernet_header::write(void* buffer, size_t bufferlen) const {
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
        throw pgen::exception(
                "pgen::ethernet_header::read: Buffer length is too small");
    }

    const eth* p = (const eth*)buffer;

    src.setbyarray(p->src);
    dst.setbyarray(p->dst);
    type = ntohs(p->type);
}


size_t ethernet_header::length() const {
    return ethernet_header::max_length;
}






ethernet::ethernet() {
    clear();
    init_headers();
}



ethernet::ethernet(const void* buffer, size_t bufferlen) : ethernet() {
    analyze(buffer, bufferlen);
}



ethernet::ethernet(const pgen::ethernet& rhs) : packet(rhs) {
    ETH = rhs.ETH;
    init_headers();
}


void ethernet::clear() {
    ETH.clear();
}




void ethernet::init_headers() {
    headers.clear();
    headers.push_back(&ETH);
}





} /* namespace pgen */

