



#include <pgen2/core/header/header.h>
#include <pgen2/exception.h>


namespace pgen {
 

ethernet_header::ethernet_header() {
    src().clear();
    dst().clear();
    type() = 0;
}

const macaddress& ethernet_header::src() const {
    return _src;      
}

const macaddress& ethernet_header::dst() const {
    return _dst;      
}

const uint16_t& ethernet_header::type() const {
    return _type;      
}

macaddress& ethernet_header::src() {
    return _src;       
}

macaddress& ethernet_header::dst() {
    return _dst;      
}

uint16_t& ethernet_header::type() {
    return _type;      
}

void ethernet_header::write() {
    struct eth {
        uint8_t dst[pgen::macaddress::length];
        uint8_t src[pgen::macaddress::length];
        uint16_t type;
    };

    eth* p = (eth*)_raw;
    for (size_t i=0; i<pgen::macaddress::length; i++) {
        p->src[i] = src().get_octet(i+1);
        p->dst[i] = dst().get_octet(i+1);
    }
    p->type = htons(type());
}

const uint8_t* ethernet_header::raw() const {
    return _raw;
}

size_t ethernet_header::read(const void* buffer, size_t buffer_len) {
    struct eth {
        uint8_t dst[pgen::macaddress::length];
        uint8_t src[pgen::macaddress::length];
        uint16_t type;
    };

    if (buffer_len < sizeof(eth)) {
        throw pgen::exception("pgen::ethernet_header::read: Buffer length is too small");
    }

    const eth* p = (const eth*)buffer;

    src().setbyarray(p->src);
    dst().setbyarray(p->dst);
    type() = ntohs(p->type);
    
    return sizeof(eth);      
}

size_t ethernet_header::length() const {
    return ethernet_header::max_length;
}




} /* pgen */
