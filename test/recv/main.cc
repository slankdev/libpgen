#include <pgen2.h>
#include <pgen.h>
#include <exception>

const char* dev = "en0";
int main() {
    try {
        pgen::net_stream s;
        s.open(dev, pgen::open_mode::netif);

        while (1) {
            uint8_t buf[4096];
            size_t recvlen = s.recv(buf, sizeof(buf));

            pgen::ethernet pack;
            pack.analyze(buf, recvlen);
            
            if (pack.ETH.type() == 0x0806) {
                pack.summary();
            }
        }
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
