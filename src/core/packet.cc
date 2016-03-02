


#include <assert.h>
#include <pgen2/data_container.h>
#include <pgen2/core/packet.h>
#include <pgen2/util.h>



namespace pgen {



packet::packet() {}
packet::packet(const packet& rhs) {
    _type = rhs._type;
    _raw  = rhs._raw;
    // _header_len = rhs._header_len; // ERASE 
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

    for (const pgen::header* ph : headers) {
        header_length += ph->length();
    }
    
    // ERASE
    // for (header_vect::const_iterator it=headers.begin(); 
    //                                     it!=headers.end(); it++) {
    //     header_length += (*it)->length();
    // }

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

    // ERASE
    // for (header_vect::iterator it=headers.begin(); 
    //                                     it!=headers.end(); it++) {
    //     (*it)->read(pointer, bufferlen);
    //     pointer   += (*it)->length();
    //     bufferlen -= (*it)->length();
    // }

    set_contents(pointer, bufferlen);
}


} /* namespace pgen */
