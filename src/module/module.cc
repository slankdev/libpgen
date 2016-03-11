

#include <pgen2/core/packet.h>
#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/protocol/ip.h>
#include <pgen2/core/protocol/ipv6.h>

#include <pgen2/io/stream.h>
#include <string.h>
#include <stdio.h>



namespace pgen {
namespace module {



pgen::packet_type detect(const void* buffer, size_t bufferlen) {
    pgen::packet_type type = pgen::packet_type::ethernet;

    uint8_t* p = (uint8_t*)buffer;
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

        // return func(ip.protocol);
        if (ip.protocol == 1)
            goto analyze_icmp;
        if (ip.protocol == 6)
            goto analyze_tcp;
        if (ip.protocol == 17)
            goto analyze_udp;
        else 
            return type;

    } else if (eth.type == 0x86dd) {
        type = pgen::packet_type::ipv6;

        pgen::ipv6_header ipv6h;
        ipv6h.read(p, bufferlen);
        p += ipv6h.length();
        bufferlen -= ipv6h.length();

        // return func(ip.protocol);

        if (ipv6h.next_header == 1)
            goto analyze_icmp;
        if (ipv6h.next_header == 6)
            goto analyze_tcp;
        if (ipv6h.next_header == 17)
            goto analyze_udp;
        else 
            return type;

    } else if (eth.type == 0x0806) {
        type = pgen::packet_type::arp;
        return type;
    } else {
        type = pgen::packet_type::ethernet;   
        return type;
    }

analyze_icmp:
    type = pgen::packet_type::icmp;
    return type;
analyze_tcp:
    type = pgen::packet_type::tcp;
    return type;
analyze_udp:
    type = pgen::packet_type::udp;
    return type;

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
