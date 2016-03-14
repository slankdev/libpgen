

#pragma once

#include <stddef.h>
#include <stdint.h>


namespace pgen {



uint16_t checksum(const void *data, size_t len);

void hex(const void* buffer, size_t bufferlen);
void bin(const void* buffer, size_t bufferlen);


} /* namespace pgen */
