

#pragma once


#include <assert.h>
#include <pgen2/util.h>
#include <pgen2/data_container.h>




namespace pgen {


enum class packet_type {
    unknown,
    ethernet, 
    arp,
    ip,
    icmp,
    tcp,
    udp
};




class packet {
    protected:
        pgen::packet_type _type;
        pgen::types::data_container  _raw;
        size_t _header_len;
        static const size_t max_header_len = 128; /* [byte] */

    public:

        packet_type type() const;
        void set_contents(const void* buffer, size_t buflen);
        void hex() const;

        const uint8_t* raw() const;
        const uint8_t* contents() const;
        size_t length() const;

        virtual size_t header_length() const = 0;
        virtual void clear() = 0;
        virtual void compile() = 0;
        virtual void analyze(const void* buffer, size_t buffer_len) = 0;
        virtual void summary(bool moreinfo=false) const = 0;
};









} /* namespace pgen */


