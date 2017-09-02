

/**
 * @file packet.h
 * Definition about base packet class. We hope to you extending
 * new packet class with this class.
 **/


#pragma once


#include <assert.h>
#include <pgen/util.h>
#include <pgen/core/data_container.h>

#include <vector>
#include <pgen/core/header.h>
#include <pgen/io/stream.h>


namespace pgen {


/**
 * @enum packet_type
 * This enum is packet_type about protocol type.
 **/
enum class packet_type {
    unknown,
    ethernet,
    arp,
    ip,
    ipv6,
    icmp,
    tcp,
    udp
};




/**
 * This class provides packet functionality.
 * This is base class, so developers must implement new class
 * inherited this if they want to add new protocol packet class.
 *
 * <h3>Member Variables</h3>
 * The class specilized in one protocol, has header classes
 * of the following protocol.
 * For example, ICMP packet class has ICMP header class, IP header
 * class and Ethernet header class.
 **/
class packet {
    protected:
        pgen::packet_type _type;
        pgen::types::data_container  _raw;
        static const size_t max_header_len = 128; /* [byte] */

        using header_vect = std::vector<pgen::header*>;
        header_vect headers;

        /**
         * Init headers array using compile() and analyze().
         * Developer must override this.
         **/
        virtual void init_headers() = 0;

    public:

        friend pgen::base_stream& operator << (pgen::base_stream& s, const pgen::packet& pack);

        packet();
        packet(const packet& rhs);

        /**
         * Return packet type as pgen::packet_type.
         * About pgen::packet_type, refer pgen::packet_type
         **/
        packet_type type() const;

        /**
         * Set packet contents.
         **/
        void set_contents(const void* buffer, size_t buflen);

        /**
         * Print hexdump
         **/
        void hex() const;

        /**
         * Return packet raw data pointer.
         * User can't read and write this area.
         * A value returned this function depends on the
         * compile();
         **/
        const uint8_t* raw() const;

        /**
         * Return packet content data.
         * User can't read and write this area.
         **/
        const uint8_t* contents() const;

        /**
         * Return packet length.
         * A value returned this function doesn't depend
         * on the compile();
         **/
        size_t length() const;

        /**
         * return total header length.
         * A value returned this function doesn't depend
         * on the compile();
         **/
        size_t header_length() const;

        /**
         * Build binary to private mamever variable, _rar.
         * Call header-class::write() inside this function.
         * This function update only raw data.
         **/
        void compile();

        /**
         * Analyze binary as packet.
         * Call header-class::read() inside this function.
         * This function update only protocol member variables.
         * This function doesn't update raw data.
         **/
        void analyze(const void* buffer, size_t bufferlen);

        /**
         * Re init values.
         * Developer must override this.
         **/
        virtual void clear() = 0;


        pgen::packet& operator = (const pgen::packet& rhs);
};



} /* namespace pgen */





/* Func IMple */


#include <assert.h>
#include <pgen/core/data_container.h>
#include <pgen/core/packet.h>
#include <pgen/util.h>
#include <pgen/exception.h>



namespace pgen {


inline pgen::base_stream& operator << (pgen::base_stream& s, const pgen::packet& pack) {
    s.send(pack.raw(), pack.length());
    return s;
}



inline packet::packet() {}
inline packet::packet(const packet& rhs) {
    _type = rhs._type;
    _raw  = rhs._raw;
}


inline void packet::set_contents(const void* buffer, size_t buflen) {
    _raw.set_content(buffer, buflen);
}
inline const uint8_t* packet::contents() const {
    assert(_raw.size() >= pgen::packet::max_header_len);
    return _raw.data() + pgen::packet::max_header_len;
}


inline const uint8_t* packet::raw() const {
    size_t headerlen = header_length();
    assert(headerlen <= pgen::packet::max_header_len);
    return contents() - headerlen;
}


inline size_t packet::length() const {
    assert(_raw.size() >= pgen::packet::max_header_len);
    return _raw.size() - pgen::packet::max_header_len + header_length();
}


inline size_t packet::header_length() const {
    size_t header_length = 0;
    for (const pgen::header* ph : headers)
        header_length += ph->length();

    return header_length;
}


inline packet_type packet::type() const {
    return _type;
}


inline void packet::hex() const {
    pgen::hex(raw(), length());
}



inline void packet::compile() {
    uint8_t* pointer = _raw.data() + _raw.pivot();

    for (auto it=headers.rbegin(); it!=headers.rend(); ++it) {
        (*it)->write(pointer-((*it)->length()), (*it)->length());
        pointer -= (*it)->length();
    }
}



inline void packet::analyze(const void* buffer, size_t bufferlen) {
    const uint8_t* pointer = (uint8_t*)buffer;

    for (pgen::header* ph : headers) {
        ph->read(pointer, bufferlen);
        pointer   += ph->length();
        bufferlen -= ph->length();
    }

    set_contents(pointer, bufferlen);
}


inline pgen::packet& packet::operator = (const pgen::packet& rhs) {
    init_headers();
    if (rhs.headers.size() != headers.size())
        throw pgen::exception("pgen::packet::operator=: not same packet type");

    for (size_t i=0; i<headers.size(); i++)
        *headers[i] = *(rhs.headers[i]);

    return *this;
}



} /* namespace pgen */









