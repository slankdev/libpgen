
#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/ipaddress.h>





namespace pgen {



class ipv4_header : public header {
    public:
        static const size_t min_length = 20;
        static const size_t max_length = 60;

        uint8_t      hlen; // this is special field, and 4bit field
        uint8_t      tos;
        uint16_t     tot_len; // this is special field 
        uint16_t     id;
        uint16_t     frag_off;
        uint8_t      ttl;
        uint8_t      protocol;
        uint16_t     check;
        ipv4address  src;
        ipv4address  dst;

    private:
        uint8_t option[max_length-min_length];

    public:
        void set_option(const void* buffer, size_t bufferlen);

        void clear() override;
        void summary(bool moreinfo=false) const override;
        void write(void* buffer, size_t buffer_len) const override;
        void read(const void* buffer, size_t buffer_len) override;
        size_t length() const override;
};



// version4 only yet
class ipv4 : public packet {
    private:
        void init_headers() override;
    public:
        pgen::ethernet_header ETH;
        pgen::ipv4_header IP;

        ipv4();
        ipv4(const void* buffer, size_t bufferlen);
        ipv4(const pgen::ipv4& rhs);

        void clear() override;
    
};


} /* namespace pgen */
