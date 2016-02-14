

#include <vector> 
#include <sstream>
#include <assert.h>
#include <pgen2/core/packet.h>
#include <pgen2/io/util.h>



namespace pgen {
namespace core {




void  packet::set_contents(const void* buffer, size_t buflen) {
    _raw.set_content(buffer, buflen);       
}
const uint8_t* packet::contents() const {
    assert(_raw.size() >= pgen::core::packet::max_header_len);
    return _raw.data() + pgen::core::packet::max_header_len;  
}


const uint8_t* packet::raw() const {
    size_t headerlen = header_length();
    assert(headerlen <= pgen::core::packet::max_header_len);
    return contents() - headerlen;  
}


size_t packet::length() const {
    assert(_raw.size() >= pgen::core::packet::max_header_len);
    return _raw.size() - pgen::core::packet::max_header_len + header_length();
}


packet_type packet::get_type() const {
    return _type;   
}
void packet::hex() const {
    pgen::io::util::hex(raw(), length());
}



} /* namespace core */
} /* namespace pgen */
