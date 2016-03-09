
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
        void write(void* buffer, size_t bufferlen) const override;
        void read(const void* buffer, size_t bufferlen) override;
        size_t length() const override;
};



class ethernet : public packet {
    private:
        void init_headers() override;
    public:
        enum class type : std::uint16_t {
            ip   = 0x0800,
            arp  = 0x0806,
            ipv6 = 0x86dd
        };

        pgen::ethernet_header ETH;

        ethernet();
        ethernet(const void* buffer, size_t bufferlen);
        ethernet(const pgen::ethernet& rhs); 
        // pgen::ethernet& operator = (const pgen::ethernet& rhs); //ERASE

        void clear() override;

};


} /* namespace pgen */

