
#include <pgen2.h>
#include <pgen2/core/header.h>


namespace pgen {
namespace core {
namespace header {
 

ethernet::ethernet() {
    src().clear();
    dst().clear();
    type() = 0;
}


const macaddress& ethernet::src() const {
    return _src;      
}

const macaddress& ethernet::dst() const {
    return _dst;      
}

const uint16_t& ethernet::type() const {
    return _type;      
}

macaddress& ethernet::src() {
    return _src;       
}


macaddress& ethernet::dst() {
    return _dst;      
}

uint16_t& ethernet::type() {
    return _type;      
}



size_t ethernet::write_header(void* buffer, size_t buffer_len) {
    
    struct eth {
        uint8_t dst[6];
        uint8_t src[6];
        uint16_t type;
    };

    struct eth* p = (struct eth*)buffer;
    for (int i=0; i<6; i++) {
        p->src[i] = src().get_octet(i+1);
        p->dst[i] = dst().get_octet(i+1);
    }
    p->type = htons(type());

    return sizeof(struct eth);
}

size_t ethernet::read_header(const void* buffer, size_t buffer_len) {
    struct eth {
        uint8_t dst[6];
        uint8_t src[6];
        uint16_t type;
    };
    const struct eth* p = (const struct eth*)buffer;

    src().setmacbyarray(p->src);
    dst().setmacbyarray(p->dst);
    type() = ntohs(p->type);
    
    return 1;      
}

} /* header */
} /* core */
} /* pgen */
