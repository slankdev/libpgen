


#pragma once

#include <pgen2/core/packet.h>
#include <stddef.h>


namespace pgen {
namespace module {



bool detect_ethernet(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* type=nullptr);
bool detect_arp(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr);
bool detect_ipv6(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* proto=nullptr);
bool detect_ipv4(const void* buffer, size_t bufferlen, size_t* hdrlen=nullptr, uint16_t* proto=nullptr);
bool detect_icmpv4(const void* buffer, size_t bufferlen);


int check(
        pgen::packet* pack, const char* filename, 
        void (*callback)(const void*,size_t)=nullptr);


} /* namespace module */
} /* namespace pgen */
