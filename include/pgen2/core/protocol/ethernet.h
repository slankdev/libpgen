
#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/macaddress.h>




namespace pgen {



class ethernet_header : public header {
    public:
        static const size_t min_length 
            = pgen::macaddress::length*2+sizeof(uint16_t);
        static const size_t max_length 
            = min_length;

        macaddress dst;
        macaddress src;
        uint16_t   type;
    
        void clear() override;
        void summary(bool moreinfo=false) const override;
        void write(void* buffer, size_t buffer_len) const override;
        void read(const void* buffer, size_t buffer_len) override;
        size_t length() const override;
};



class ethernet : public packet {
    private:
        void init_headers() override;
    public:
        pgen::ethernet_header ETH;

        ethernet();
        ethernet(const void* buffer, size_t bufferlen);
        ethernet(const pgen::ethernet& rhs); 

        void clear() override;

};


} /* namespace pgen */

