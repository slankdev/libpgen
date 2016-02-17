
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
        
} /* namespace arch */
} /* namespace pgen */




