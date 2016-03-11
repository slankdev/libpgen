

#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/ipaddress.h>
#include <pgen2/core/macaddress.h>



namespace pgen {



class arp_header : public header {
    public:
        static const size_t min_length = 28;
        static const size_t max_length = 28;
    
        uint16_t hwtype;
        uint16_t ptype;
        uint8_t  hwlen;
        uint8_t  plen;
        uint16_t operation;
        pgen::macaddress hwsrc;
        pgen::ipv4address psrc;
        pgen::macaddress hwdst;
        pgen::ipv4address pdst;

        void clear() override;
        void summary(bool moreinfo=false) const override;
        void write(void* buffer, size_t bufferlen) const override;
        void read(const void* buffer, size_t bufferlen) override;
        size_t length() const override;
};



class arp : public packet {
    private:
        void init_headers() override;
    public:
        struct operation {
            static const uint16_t request = 1;
            static const uint16_t reply   = 2;
        };

        pgen::ethernet_header ETH;
        pgen::arp_header      ARP;

        arp();
        arp(const void* buffer, size_t bufferlen);
        arp(const pgen::arp& rhs); 

        void clear() override;
};


} /* namespace pgen */

