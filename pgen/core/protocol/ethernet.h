
#pragma once

#include <pgen/core/packet.h>
#include <pgen/core/header.h>
#include <pgen/core/macaddress.h>




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
        pgen::ethernet_header ETH;

        ethernet();
        ethernet(const void* buffer, size_t bufferlen);
        ethernet(const pgen::ethernet& rhs);

        void clear() override;

};





/************************************************************************\
 *                                                                      *
 * Following code is Function Implementation                            *
 *                                                                      *
\************************************************************************/


#include <pgen/exception.h>
#include <arpa/inet.h>



struct eth {
    uint8_t dst[pgen::macaddress::length];
    uint8_t src[pgen::macaddress::length];
    uint16_t type;
};



inline  void ethernet_header::clear() {
    src  = "00:00:00:00:00:00";
    dst  = "00:00:00:00:00:00";
    type = 0x0000;
}


inline  void ethernet_header::summary(bool moreinfo) const {
    printf("Ethernet [%s -> %s type=0x%04x] \n",
            src.str().c_str(),
            dst.str().c_str(), type);

    if (moreinfo) {
        printf(" - source      : %s \n", src.str().c_str());
        printf(" - destination : %s \n", dst.str().c_str());
        printf(" - type        : 0x%04x \n", type);
    }
}


inline  void ethernet_header::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < min_length) {
        throw pgen::exception("pgen::ethernet_header::write: buflen is too small");
    }

    struct eth* p = reinterpret_cast<eth*>(buffer);
    src.copytoarray(p->src);
    dst.copytoarray(p->dst);
    p->type = htons(type);
}



inline  void ethernet_header::read(const void* buffer, size_t buffer_len) {

    if (buffer_len < sizeof(eth)) {
        throw pgen::exception(
                "pgen::ethernet_header::read: Buffer length is too small");
    }

    const eth* p = reinterpret_cast<const eth*>(buffer);
    src.setbyarray(p->src);
    dst.setbyarray(p->dst);
    type = ntohs(p->type);
}


inline  size_t ethernet_header::length() const {
    return ethernet_header::max_length;
}






inline  ethernet::ethernet() {
    clear();
    init_headers();
}



inline  ethernet::ethernet(const void* buffer, size_t bufferlen) : ethernet() {
    analyze(buffer, bufferlen);
}



inline  ethernet::ethernet(const pgen::ethernet& rhs) : packet(rhs) {
    ETH = rhs.ETH;
    init_headers();
}



inline  void ethernet::clear() {
    ETH.clear();
}




inline  void ethernet::init_headers() {
    headers = {&ETH};
}





} /* namespace pgen */


