



#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "en0";
const char* out = "out.pcap";
pgen::net_stream net;

void recv_arp() {
    while(1) {
        uint8_t buf[1000];
        size_t recvlen = net.recv(buf, sizeof buf);

        pgen::ethernet eth(buf, recvlen);
        if (eth.ETH.type == 0x0806) {
            pgen::arp arp(buf, recvlen);
            if (arp.ARP.operation == 2) {
                arp.ARP.summary();
                return ;
            }
        }
    }
}


int main() {
    try {
        pgen::arp pack;
        pack.ARP.hwsrc.setbydev(dev);
        pack.ARP.hwdst.setbcast();
        pack.ARP.psrc.setbydev(dev);
        pack.ARP.pdst = "10.210.124.1";
        pack.ETH.src = pack.ARP.hwsrc;
        pack.ETH.dst = pack.ARP.hwdst;

        pack.compile();
        pack.ARP.summary();
    
        net.open(dev, pgen::open_mode::netif);
        net << pack;

        recv_arp();
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}


