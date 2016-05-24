


#pragma once

#include <pgen2/core/packet.h>
#include <stddef.h>


namespace pgen {
namespace module {


bool detect_arp(const void* buffer, size_t bufferlen);
bool detect_ipv6(const void* buffer, size_t bufferlen);


int check(
        pgen::packet* pack, const char* filename, 
        void (*callback)(const void*,size_t)=nullptr);


} /* namespace module */
} /* namespace pgen */
