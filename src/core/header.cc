
#include <pgen2.h>
#include <pgen2/core/header.h>


namespace pgen {
namespace core {
 

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



size_t ethernet_header::write(void* buffer, size_t buffer_len) {
    struct eth {
        uint8_t dst[6];
        uint8_t src[6];
        uint16_t type;
    };

    if (buffer_len < sizeof(struct eth)) {
        printf("あとでthrow");
        return 1;
        // throw
    }

    struct eth* p = (struct eth*)buffer;
    for (int i=0; i<6; i++) {
        p->src[i] = src().get_octet(i+1);
        p->dst[i] = dst().get_octet(i+1);
    }
    p->type = htons(type());

    return sizeof(struct eth);
}

size_t ethernet_header::read(const void* buffer, size_t buffer_len) {
    struct eth {
        uint8_t dst[6];
        uint8_t src[6];
        uint16_t type;
    };

    if (buffer_len < sizeof(struct eth)) {
        printf("あとでthrow");
        return 1;
        // throw
    }

    const struct eth* p = (const struct eth*)buffer;

    src().setbyarray(p->src);
    dst().setbyarray(p->dst);
    type() = ntohs(p->type);
    
    return 1;      
}

} /* core */
} /* pgen */
