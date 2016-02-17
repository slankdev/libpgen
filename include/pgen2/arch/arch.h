
#pragma once

#ifdef __linux
#define __PGEN_LINUX
#endif

#ifdef __APPLE__
#define __PGEN_OSX
#endif



#include <stdint.h>

namespace pgen {
namespace arch {


void getmacbydev(const char* dev, uint8_t mac[6]);
void getipv4bydev(const char* dev, uint8_t ip[4]);
void getipv6bydev(const char* dev, uint16_t ip[8]);

#if defined(__PGEN_LINUX)
int open_rawsock(const char* ifname);
#endif

#if defined(__PGEN_OSX)
int open_bpf(const char* ifname);
#endif


} /* namespace arch */
} /* namespace pgen */




