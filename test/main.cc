

#include <pgen2.h>
const char* dev  = "en0";
const char* file = "out.pcap";

int main() {
    // uint8_t buf[] = {1,2,3,4};
    //
    // pgen::ipv4address a;
    // a.setbyarray(buf);
    //
    // printf("address: %s \n", a.str().c_str());
    //
    // uint8_t aa[4];
    // a.copytoarray(aa);
    // pgen::hex(aa, 4);
    //
    //return -1;
    
    try {
        pgen::pcap_stream s;
        s.open(file, pgen::open_mode::pcap_write);

        uint8_t data[] = {0x88, 0xaa, 0xbb, 0xdd};

        pgen::ipv4 pack;
        pack.set_contents(data, sizeof(data));
        pack.ETH.src.setbydev(dev);
        pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
        pack.IP.hlen = 5;
        pack.IP.tot_len = (pack.IP.hlen<<2) + 4;
        pack.IP.protocol = 1;
        pack.IP.src.setbydev(dev);
        pack.IP.dst = "1.2.3.4";

        pack.IP.summary();
        // printf("header length = %zd \n", pack.header_length());
        // printf("packet length = %zd \n", pack.length());

        // pgen::hex(pack.raw(), pack.length());
        pack.compile();
        pack.hex();
        // pgen::hex(pack.raw(), pack.length());
    
        s.send(pack.raw(), pack.length());
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}



