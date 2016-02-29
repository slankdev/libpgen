


#include <assert.h>
#include <pgen2/data_container.h>
#include <pgen2/core/packet.h>
#include <pgen2/util.h>



namespace pgen {


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


packet_type packet::type() const {
    return _type;   
}
void packet::hex() const {
    pgen::hex(raw(), length());
}



void packet::compile() {
    uint8_t* pointer = _raw.data() + _raw.pivot();
    
    for (std::vector<pgen::header*>::reverse_iterator it=headers.rbegin(); 
                                                     it!=headers.rend(); it++) {
    printf("test \n");
        (*it)->write(pointer-((*it)->length()), (*it)->length());
        pointer -= (*it)->length();
    }
}



void packet::analyze(const void* buffer, size_t bufferlen) {
    const uint8_t* pointer = (uint8_t*)buffer;

    for (std::vector<pgen::header*>::iterator it=headers.begin(); it!=headers.end(); it++) {
        (*it)->read(pointer, bufferlen);
        pointer   += (*it)->length();
        bufferlen -= (*it)->length();
    }

    set_contents(pointer, bufferlen);
}


} /* namespace pgen */
