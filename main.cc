

#include <pgen2.h>


int main() {
    pgen::core::macaddress m = "00:11:22:33:44:55";

    pgen::core::ethernet pack;

     printf(" - source      : %s \n", pack.ETH.src().get_str().c_str());
     printf(" - destination : %s \n", pack.ETH.dst().get_str().c_str());
     printf(" - type        : 0x%04x \n", pack.ETH.type());
}

