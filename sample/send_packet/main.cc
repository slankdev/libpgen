
#include <stdio.h>
#include <pgen2.h>

const char* dev = "eth0";

int main()
{
    pgen::arp pack;
    pack.ETH.src.setbydev(dev);
    pack.ETH.dst.setbcast();
    pack.ARP.operation = 1;
    pack.ARP.hwsrc = pack.ETH.src;
    pack.ARP.psrc  = "192.168.1.111";
    pack.ARP.hwdst = pack.ETH.dst;
    pack.ARP.pdst  = "192.168.1.1";
    
    pack.compile();
    pack.hex();

    pgen::net_stream net(dev, pgen::open_mode::netif);
    for (;;)
        net << pack;
}
