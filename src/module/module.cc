

#include <pgen2/core/packet.h>
#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/protocol/ip.h>
#include <pgen2/core/protocol/ipv6.h>

#include <pgen2/io/stream.h>
#include <string.h>
#include <stdio.h>



namespace pgen {
namespace module {


static pgen::packet_type detect_L4(uint8_t protocol, const void* buffer, size_t bufferlen) {
    // ERASE
    if (buffer == NULL || bufferlen == 0)
        exit(-1);
    
    if (protocol == pgen::ipv4::protocol::icmp)
        return pgen::packet_type::icmp;
    else if (protocol == pgen::ipv4::protocol::tcp)
        return pgen::packet_type::tcp;
    else if (protocol == pgen::ipv4::protocol::udp)
        return pgen::packet_type::udp;

    return pgen::packet_type::ip;
}


pgen::packet_type detect(const void* buffer, size_t bufferlen) {
    pgen::packet_type type = pgen::packet_type::ethernet;

    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);
    pgen::ethernet_header eth;
    eth.read(p, bufferlen);
    p += eth.length();
    bufferlen -= eth.length();

    if (eth.type == 0x0800) {
        type = pgen::packet_type::ip;

        pgen::ipv4_header ip;
        ip.read(p, bufferlen);
        p += ip.length();
        bufferlen -= ip.length();

        return detect_L4(ip.protocol, p, bufferlen);

    } else if (eth.type == 0x86dd) {
        type = pgen::packet_type::ipv6;

        pgen::ipv6_header ipv6h;
        ipv6h.read(p, bufferlen);
        p += ipv6h.length();
        bufferlen -= ipv6h.length();

        return detect_L4(ipv6h.next_header, p, bufferlen);

    } else if (eth.type == 0x0806) {
        type = pgen::packet_type::arp;
        return type;
    } else {
        type = pgen::packet_type::ethernet;   
        return type;
    }

}



int check(pgen::packet* pack, const char* filename, 
                    void (*callback)(const void*,size_t)) {

    pgen::pcap_stream handle;
    handle.open(filename, pgen::open_mode::pcap_read);

    int count = 0; 
    while (1) {
        if (handle.eof())
            break;

        uint8_t data[10000];
        size_t datalen = handle.recv(data, sizeof(data));

        pack->analyze(data, datalen);
        pack->compile();
    
        if (datalen != pack->length()) {
            printf("binary operation havs length error \n");
            if (callback) callback(data, datalen);
        }else if (memcmp(pack->raw(), data, datalen) == 0) {
            printf("binary operation is successed\n");
        } else {
            printf("binary operation havs binary error \n");
            if (callback) callback(data, datalen);
        }
        count++;
    }
    return count;
}

 


} /* namespace module */
} /* namespace pgen */
