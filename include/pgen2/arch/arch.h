
#pragma once

#ifdef __linux
#define __PGEN_LINUX
#endif

#ifdef __APPLE__
#define __PGEN_MAXOS
#endif



#include <stdint.h>

namespace pgen {
namespace arch {


void getmacbydev(const char* dev, uint8_t mac[6]);
void getipv4bydev(const char* dev, uint8_t ip[4]);
void getipv6bydev(const char* dev, uint16_t ip[8]);


} /* namespace arch */
} /* namespace pgen */




