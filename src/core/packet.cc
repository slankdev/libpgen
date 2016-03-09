


#include <assert.h>
#include <pgen2/data_container.h>
#include <pgen2/core/packet.h>
#include <pgen2/util.h>
#include <pgen2/exception.h>



namespace pgen {


pgen::base_stream& operator << (pgen::base_stream& s, const pgen::packet& pack) {
    s.send(pack.raw(), pack.length()); 
    return s;
}



packet::packet() {}
packet::packet(const packet& rhs) {
    _type = rhs._type;
    _raw  = rhs._raw;
}


void packet::set_contents(const void* buffer, size_t buflen) {
    _raw.set_content(buffer, buflen);       
}
const uint8_t* packet::contents() const {
    assert(_raw.size() >= pgen::packet::max_header_len);
    return _raw.data() + pgen::packet::max_header_len;  
}


const uint8_t* packet::raw() const {
    size_t headerlen = header_length();
    assert(headerlen <= pgen::packet::max_header_len);
    return contents() - headerlen;  
}


size_t packet::length() const {
    assert(_raw.size() >= pgen::packet::max_header_len);
    return _raw.size() - pgen::packet::max_header_len + header_length();
}


size_t packet::header_length() const {
    size_t header_length = 0;
    for (const pgen::header* ph : headers)
        header_length += ph->length();

    return header_length;
}


packet_type packet::type() const {
    return _type;   
}
void packet::hex() const {
    pgen::hex(raw(), length());
}



void packet::compile() {
    uint8_t* pointer = _raw.data() + _raw.pivot();

    for (auto it=headers.rbegin(); it!=headers.rend(); ++it) {
        (*it)->write(pointer-((*it)->length()), (*it)->length());
        pointer -= (*it)->length();
    }
}



void packet::analyze(const void* buffer, size_t bufferlen) {
    const uint8_t* pointer = (uint8_t*)buffer;

    for (pgen::header* ph : headers) {
        ph->read(pointer, bufferlen);
        pointer   += ph->length();
        bufferlen -= ph->length();
    }

    set_contents(pointer, bufferlen);
}


pgen::packet& packet::operator = (const pgen::packet& rhs) {
    init_headers();
    if (rhs.headers.size() != headers.size())
        throw pgen::exception("pgen::packet::operator=: not same packet type");

    for (size_t i=0; i<headers.size(); i++)
        *headers[i] = *(rhs.headers[i]);

    return *this;
}



} /* namespace pgen */
