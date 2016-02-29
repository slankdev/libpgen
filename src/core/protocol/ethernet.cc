

#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/exception.h>

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







ethernet::ethernet() {
    headers.push_back(&ETH);
    clear();
}
ethernet::ethernet(const void* buffer, size_t bufferlen) : ethernet() {
    analyze(buffer, bufferlen);
}


void ethernet::clear() {
    ETH.src  = "00:00:00:00:00:00";
    ETH.dst  = "00:00:00:00:00:00";
    ETH.type = 0x0000;
}


size_t ethernet::header_length() const {
    return ETH.length();
}


// void ethernet::compile() {
// #if 0
//     ETH.write();
//     _raw.write_before(_raw.pivot(), ETH.raw(), ETH.length());
// #else
//     ETH.write(_raw.data()+_raw.pivot()-ETH.length() , _raw.pivot()-ETH.length());
// #endif
// }
//
//
//
// void ethernet::analyze(const void* buffer, size_t buffer_len) {
//     if (buffer_len < pgen::ethernet_header::min_length)
//         throw pgen::exception("pgen::ethernet::analyze: Buffer length is too small");
//
//     ETH.read(buffer, buffer_len); 
//     _header_len = ETH.length();
//     set_contents((uint8_t*)buffer + _header_len, buffer_len - _header_len);
// }


void ethernet::summary(bool moreinfo) const {
    printf("Ethernet [%s -> %s type=0x%04x] \n", 
            ETH.src.str().c_str(),
            ETH.dst.str().c_str(), ETH.type);

    if (moreinfo) {
        printf(" - source      : %s \n", ETH.src.str().c_str());
        printf(" - destination : %s \n", ETH.dst.str().c_str());
        printf(" - type        : 0x%04x \n", ETH.type);
    } 
}




} /* namespace pgen */

