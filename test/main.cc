

#include <pgen2.h>
const char* dev  = "en0";
const char* file = "out.pcap";

int main() {
    try {
        pgen::pcap_stream s;
        s.open(file, pgen::open_mode::pcap_write);

        uint8_t data[] = {0x08, 0x00, 0x00, 0x00};

        pgen::ipv4 pack;
        pack.set_contents(data, sizeof(data));
        pack.ETH.src.setbydev(dev);
        pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
        pack.IP.hlen = 5;
        pack.IP.tot_len = (pack.IP.hlen<<2) + 4;
        pack.IP.protocol = 1;
        pack.IP.src.setbydev(dev);
        pack.IP.dst = "1.2.3.4";

        pack.compile();
        pack.ETH.summary(true);
        pack.IP.summary(true);
    
        s.send(pack.raw(), pack.length());
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}



