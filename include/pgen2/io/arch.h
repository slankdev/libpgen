
#pragma once

#include <stdint.h>

namespace pgen {
namespace io {
namespace arch {

void getmacbydev(const char* dev, uint8_t mac[6]);
        
} /* namespace arch */
} /* namespace io */
} /* namespace pgen */




