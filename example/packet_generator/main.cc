#include <pgen2.h>

int main() 
{
    try {

        pgen::arp pack;
        pack.compile();
    
        pgen::pcap_stream pcap("out.pcap", pgen::open_mode::pcap_write);
        pcap << pack;
    
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
