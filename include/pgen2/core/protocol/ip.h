
#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/protocol/ethernet.h>





namespace pgen {



class ipv4_header : public header {
    public:
        static const size_t min_length
            = 12+pgen::ipv4address::length*2;
        static const size_t max_length
            = min_length+40;

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
        uint8_t option[max_length-min_length];


        void clear();
        void summary(bool moreinfo=false) const;
        void write(void* buffer, size_t buffer_len) const;
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};



// version4 only yet
class ipv4 : public packet {
    private:
    public:
        pgen::ethernet_header ETH;
        pgen::ipv4_header IP;

        ipv4();
        ipv4(const void* buffer, size_t bufferlen);
        ipv4(const pgen::ipv4& rhs); // TODO check this function

        size_t header_length() const override;
        void clear() override;

        // void summary(bool moreinfo=false) const override;
        // void compile() override;
        // void analyze(const void* buffer, size_t bufferlen) override;
};


} /* namespace pgen */
