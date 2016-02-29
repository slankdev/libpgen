
#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/protocol/ethernet.h>
// #include <pgen2/core/header/header.h>





namespace pgen {



class ethernet_header : header {
    public:
        static const size_t min_length 
            = pgen::macaddress::length*2+sizeof(uint16_t);
        static const size_t max_length 
            = min_length;

        macaddress dst;
        macaddress src;
        uint16_t   type;

        void write(void* buffer, size_t buffer_len);
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};



class ethernet : public packet {
    private:
    public:
        pgen::ethernet_header ETH;

        ethernet();
        ethernet(const void* buffer, size_t bufferlen);

        size_t header_length() const override;
        void clear() override;
        void compile() override;
        void analyze(const void* buffer, size_t bufferlen) override;
        void summary(bool moreinfo=false) const override;
};



} /* namespace pgen */

