
#pragma once

#include <stddef.h>

namespace pgen {
namespace io   {

class stream {
    size_t send(const void* buffer, size_t bufferlen);
    size_t recv(void* buffer, size_t bufferlen);
};
   
} /* namespace io   */
} /* namespace pgen */
