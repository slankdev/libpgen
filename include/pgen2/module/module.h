



#include <pgen2/core/packet.h>
#include <stddef.h>


namespace pgen {
namespace module {


pgen::packet_type detect(const void* buffer, size_t bufferlen);


int check(
        pgen::packet* pack, const char* filename, 
        void (*callback)(const void*,size_t)=nullptr);


} /* namespace module */
} /* namespace pgen */
