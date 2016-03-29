


#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/core/protocol/udp.h>
#include <string>




namespace pgen {



class zundoko_header : public header {
    public:
        static const size_t min_length = sizeof(uint16_t)*2;
        static const size_t max_length = min_length + 100;

        uint16_t type;
        uint16_t msg_len;
        std::string msg;

    public:
        void clear() override;
        void summary(bool moreinfo=false) const override;
        void write(void* buffer, size_t buffer_len) const override;
        void read(const void* buffer, size_t buffer_len) override;
        size_t length() const override;

};




class zundoko : public packet {
    private:
        void init_headers() override;

    public:
        struct type {
            static const uint16_t zun     = 0;
            static const uint16_t doko    = 1;
            static const uint16_t kiyoshi = 2;
        };

        pgen::zundoko_header ZUNDOKO;
        pgen::udp_header UDP;
        pgen::ipv4_header IP;
        pgen::ethernet_header ETH;

        zundoko();
        zundoko(const void* buffer, size_t bufferlen);
        zundoko(const pgen::zundoko& rhs);

        void clear() override;
};



} /* namespace pgen */
