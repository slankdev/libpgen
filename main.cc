

#include <pgen2.h>
#include <pgen2/exception.h>
#include <iostream>
#include <exception>

int main() {

    pgen::ethernet pack;
    try {
        pack.ETH.src().setbydev("en0");
        printf("src: %s \n", pack.ETH.src().get_str().c_str());
        printf("src[]: %02x \n", pack.ETH.src().get_octet(-12));
        // pack.ETH.dst().setbydev("en5");
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
    pack.summary();
}

    // uint8_t copy[100];
    // size_t copylen;
    //
    // uint8_t d[32] = {0x11, 0x22, 0x33, 0x44};
    // memset(d, 0xee, sizeof(d));
    // d[0] = 0x11;
    // d[1] = 0x22;
    // d[2] = 0x33;
    // d[3] = 0x44;
    //
    // pgen::ethernet pack;
    // pack.ETH.src() = "11:22:33:44:55:66";
    // pack.ETH.dst() = "aa:bb:cc:dd:ee:ff";
    // pack.ETH.type() = 0x9988;
    // pack.set_contents(d, sizeof(d));
    //
    // pack.compile();
    // pack.hex();
    //
    // copylen = pack.length();
    // memcpy(copy, pack.raw(), pack.length());
    //
    // pgen::ethernet pack2;
    // pack2.analyze(copy, copylen);
    // pack2.compile();
    // pack2.hex();
    
