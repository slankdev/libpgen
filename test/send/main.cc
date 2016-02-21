#include <pgen2.h>
#include <pgen.h>
#include <exception>

const char* dev = "en0";
int main() {
    try {
        pgen::ethernet pack;
        pack.ETH.type() = 0xffff;
        pack.compile();

        pgen::net_stream s;
        s.open(dev, pgen::open_mode::netif);
        s.send(pack.raw(), pack.length());
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
