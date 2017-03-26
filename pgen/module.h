


#pragma once

#include <pgen/core/packet.h>
#include <stddef.h>



/*
 * For IMPLE
 */
#include <pgen/core/packet.h>
#include <pgen/core/protocol/arp.h>
#include <pgen/core/protocol/ethernet.h>
#include <pgen/core/protocol/ip.h>
#include <pgen/core/protocol/ipv6.h>
#include <pgen/io/net_stream.h>
#include <pgen/io/file_stream.h>
#include <pgen/io/pcapng_stream.h>
#include <pgen/io/pcap_stream.h>
#include <string.h>
#include <stdio.h>



namespace pgen {
namespace module {



inline bool detect_ethernet(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* type=nullptr)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

    pgen::ethernet_header eth;
    eth.read(p, bufferlen);
    p += eth.length();
    bufferlen -= eth.length();

    if (hdrlen) {
        *hdrlen = eth.length();
    }
    if (type) {
        *type = eth.type;
    }

    return true;
}



inline bool detect_arp(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

    size_t eth_hlen;
    uint16_t type;
    if (detect_ethernet(p, bufferlen, &eth_hlen, &type)) {
        p += eth_hlen;
        bufferlen -= eth_hlen;
        if (type == pgen::ethernet::type::arp) {
            pgen::arp_header arp;
            arp.read(p, bufferlen);

            if (hdrlen) {
                *hdrlen = eth_hlen + arp.length();
            }
            return true;
        }
    }
    return false;
}




inline bool detect_ipv4(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* proto=nullptr)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

    size_t eth_hlen;
    uint16_t type;
    if (detect_ethernet(p, bufferlen, &eth_hlen, &type)) {
        p += eth_hlen;
        bufferlen -= eth_hlen;
        if (type == pgen::ethernet::type::ip) {
            pgen::ipv4_header ipv4;
            ipv4.read(p, bufferlen);

            if (hdrlen) {
                *hdrlen = eth_hlen + ipv4.length();
            }
            if (proto) {
                *proto = ipv4.protocol;
            }
            return true;
        }
    }
    return false;
}



inline bool detect_ipv6(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* proto=nullptr)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

    size_t eth_hlen;
    uint16_t type;
    if (detect_ethernet(p, bufferlen, &eth_hlen, &type)) {
        p += eth_hlen;
        bufferlen -= eth_hlen;
        if (type == pgen::ethernet::type::ipv6) {
            pgen::ipv6_header ipv6;
            ipv6.read(p, bufferlen);

            if (hdrlen) {
                *hdrlen = eth_hlen + ipv6.length();
            }
            if (proto) {
                *proto = ipv6.next_header;
            }
            return true;
        }
    }
    return false;
}



inline bool detect_icmpv4(const void* buffer, size_t bufferlen)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);

    size_t ip_hlen;
    uint16_t proto;
    if (detect_ipv4(p, bufferlen, &ip_hlen, &proto)) {
        p += ip_hlen;
        bufferlen -= ip_hlen;
        if (proto == pgen::ipv4::protocol::icmp) {
            return true;
        }
    }
    return false;
}




// static pgen::packet_type detect_L4(uint8_t protocol, const void* buffer, size_t bufferlen) {
//     // ERASE
//     if (buffer == nullptr || bufferlen == 0)
//         exit(-1);
//
//     if (protocol == pgen::ipv4::protocol::icmp)
//         return pgen::packet_type::icmp;
//     else if (protocol == pgen::ipv4::protocol::tcp)
//         return pgen::packet_type::tcp;
//     else if (protocol == pgen::ipv4::protocol::udp)
//         return pgen::packet_type::udp;
//
//     return pgen::packet_type::ip;
// }
//
//
// pgen::packet_type detect(const void* buffer, size_t bufferlen) {
//     pgen::packet_type type = pgen::packet_type::ethernet;
//
//     const uint8_t* p = reinterpret_cast<const uint8_t*>(buffer);
//     pgen::ethernet_header eth;
//     eth.read(p, bufferlen);
//     p += eth.length();
//     bufferlen -= eth.length();
//
//     if (eth.type == 0x0800) {
//         type = pgen::packet_type::ip;
//
//         pgen::ipv4_header ip;
//         ip.read(p, bufferlen);
//         p += ip.length();
//         bufferlen -= ip.length();
//
//         return detect_L4(ip.protocol, p, bufferlen);
//
//     } else if (eth.type == 0x86dd) {
//         type = pgen::packet_type::ipv6;
//
//         pgen::ipv6_header ipv6h;
//         ipv6h.read(p, bufferlen);
//         p += ipv6h.length();
//         bufferlen -= ipv6h.length();
//
//         return detect_L4(ipv6h.next_header, p, bufferlen);
//
//     } else if (eth.type == 0x0806) {
//         type = pgen::packet_type::arp;
//         return type;
//     } else {
//         type = pgen::packet_type::ethernet;
//         return type;
//     }
//
// }



inline int check(pgen::packet* pack, const char* filename,
                    void (*callback)(const void*,size_t)=nullptr) {

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
