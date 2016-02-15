

#include <pgen2/core/header.h>
#include <pgen2/core/packet.h>
#include <pgen2/core/ethernet.h>
#include <pgen2/exception.h>

namespace pgen {



ethernet::ethernet() {
    clear();       
}


void ethernet::clear() {
    ETH.src()  = "00:00:00:00:00:00";
    ETH.dst()  = "00:00:00:00:00:00";
    ETH.type() = 0x0000;
}


size_t ethernet::header_length() const {
    return ETH.length();
}



void ethernet::compile() {
    uint8_t buf[pgen::ethernet_header::max_length];
    _header_len = ETH.write(buf, sizeof(buf));
    _raw.write_before(_raw.pivot(), buf, _header_len);
}



void ethernet::analyze(const void* buffer, size_t buffer_len) {
    if (buffer_len < pgen::ethernet_header::min_length)
        throw pgen::exception("pgen::ethernet::analyze: Buffer length is too small");

    _header_len = ETH.read(buffer, buffer_len); 
    set_contents((uint8_t*)buffer + _header_len, buffer_len - _header_len);
}


void ethernet::summary(bool moreinfo) const {
    printf("Ethernet [%s -> %s, type=0x%04x] \n", 
            ETH.src().get_str().c_str(),
            ETH.dst().get_str().c_str(), ETH.type());

    if (moreinfo) {
        printf(" - source      : %s \n", ETH.src().get_str().c_str());
        printf(" - destination : %s \n", ETH.dst().get_str().c_str());
        printf(" - type        : 0x%04x \n", ETH.type());
    } 
}




} /* namespace pgen */

