

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
    printf("length: %d \n", 
            (int)(ETH.src().length + ETH.dst().length + sizeof(uint16_t)));
    return ETH.src().length + ETH.dst().length + sizeof(uint16_t);      
}


void ethernet::analyze(const void* buffer, size_t buffer_len) {
    printf("ethernet::analyze: is not implemented yet \n" );
    return ;   
}

void ethernet::summary(bool moreinfo) const {
    if (moreinfo) {
        printf(" Ethernet \n");
        printf(" - source      : %s \n", ETH.src().get_str().c_str());
        printf(" - destination : %s \n", ETH.dst().get_str().c_str());
        printf(" - type        : 0x%04x \n", ETH.type());

    } else {
        printf(" Ethernet [%s -> %s, type=0x%04x] \n", 
                ETH.src().get_str().c_str(),
                ETH.dst().get_str().c_str(), ETH.type());
    }


}




} /* namespace core */
} /* namespace pgen */

