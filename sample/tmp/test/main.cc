

#include <pgen2.h>
#include "tmp.h"
const char* dev = "lo";

bool is_tmp_packet(const void* _buf, size_t len)
{
    const uint8_t* buf = reinterpret_cast<const uint8_t*>(_buf);

    pgen::ethernet_header eth;
    eth.read(buf, len);
    buf += eth.length();
    len -= eth.length();
    if (eth.type != pgen::ethernet::type::ip)
        return false;
    
    pgen::ipv4_header ip;
    ip.read(buf, len);
    buf += ip.length();
    len -= ip.length();
    if (ip.protocol != pgen::ipv4::protocol::udp)
        return false;

    pgen::udp_header udp;
    udp.read(buf, len);
    buf += ip.length();
    len -= ip.length();
    if (!(udp.src==8888 || udp.dst==8888))
        return false;

    return true;
}


int main()
{
    pgen::net_stream net(dev, pgen::open_mode::netif);
    while (1) {
        uint8_t buf[10000];
        size_t recvlen = net.recv(buf, sizeof buf);

        if (is_tmp_packet(buf, recvlen))
            printf("tmp packet \n");
        else
            printf("other packet \n");
    
    }

    

    
    
}
