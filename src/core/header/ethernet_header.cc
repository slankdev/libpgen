



#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>


namespace pgen {
 

ethernet_header::ethernet_header() {
    src("00:00:00:00:00:00");
    dst("00:00:00:00:00:00");
    type(0);
}

const macaddress& ethernet_header::src() const {
    return _src;      
}
const macaddress& ethernet_header::dst() const {
    return _dst;      
}
uint16_t ethernet_header::type() const {
    return _type;      
}

void ethernet_header::src(const macaddress& a) {
    _src = a;       
}
void ethernet_header::dst(const macaddress& a) {
    _dst = a;      
}
void ethernet_header::type(uint16_t t) {
    _type = t;      
}


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
        p->src[i] = src().get_octet(i+1);
        p->dst[i] = dst().get_octet(i+1);
    }
    p->type = htons(type());
}


// void ethernet_header::write() {
//     struct eth {
//         uint8_t dst[pgen::macaddress::length];
//         uint8_t src[pgen::macaddress::length];
//         uint16_t type;
//     };
//
//     eth* p = (eth*)_raw;
//     for (size_t i=0; i<pgen::macaddress::length; i++) {
//         p->src[i] = src().get_octet(i+1);
//         p->dst[i] = dst().get_octet(i+1);
//     }
//     p->type = htons(type());
// }

// const uint8_t* ethernet_header::raw() const {
//     return _raw;
// }


void ethernet_header::read(const void* buffer, size_t buffer_len) {

    if (buffer_len < sizeof(eth)) {
        throw pgen::exception("pgen::ethernet_header::read: Buffer length is too small");
    }

    const eth* p = (const eth*)buffer;

    _src.setbyarray(p->src);
    _dst.setbyarray(p->dst);
    _type = ntohs(p->type);
}

size_t ethernet_header::length() const {
    return ethernet_header::max_length;
}




} /* pgen */
