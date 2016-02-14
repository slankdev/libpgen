

#include <pgen2/core/header.h>
#include <pgen2/core/packet.h>
#include <pgen2/core/ethernet.h>

namespace pgen {



void ethernet::compile() {
    uint8_t buf[16];
    _header_len = ETH.write(buf, sizeof(buf));
    _raw.write_before(_raw.pivot(), buf, _header_len);
}



ethernet::ethernet() {
    clear();       
}

void ethernet::clear() {
    ETH.src()  = "00:00:00:00:00:00";
    ETH.dst()  = "00:00:00:00:00:00";
    ETH.type() = 0x0000;
}


size_t ethernet::header_length() const {
    return ETH.src().length + ETH.dst().length + sizeof(uint16_t);      
}


void ethernet::analyze(const void* buffer, size_t buffer_len) {
    if (buffer_len < 1) {
        printf("length error \n");
        // throw
        return ;
    }

    _header_len = ETH.read(buffer, buffer_len); 
    this->set_contents((uint8_t*)buffer + _header_len, buffer_len - _header_len);

    return ;   
}

void ethernet::summary(bool moreinfo) const {
    if (moreinfo) {
        printf("Ethernet \n");
        printf(" - source      : %s \n", ETH.src().get_str().c_str());
        printf(" - destination : %s \n", ETH.dst().get_str().c_str());
        printf(" - type        : 0x%04x \n", ETH.type());

    } else {
        printf("Ethernet [%s -> %s, type=0x%04x] \n", 
                ETH.src().get_str().c_str(),
                ETH.dst().get_str().c_str(), ETH.type());
    }
}




} /* namespace pgen */

