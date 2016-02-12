


#include <pgen2/core/header.h>
#include <pgen2/core/packet.h>
#include <pgen2/core/ethernet.h>

namespace pgen {
namespace core {



void ethernet::compile() {
          
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
    return 16;      
}


void ethernet::analyze(const void* buffer, size_t buffer_len) {
        
}
void ethernet::summary(bool moreinfo) const {
        
}
void ethernet::help() const {
        
}


} /* namespace core */
} /* namespace pgen */

