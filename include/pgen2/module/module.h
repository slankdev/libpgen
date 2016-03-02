
#include <pgen2/core/packet.h>


namespace pgen {
namespace module {


int check(
        pgen::packet* pack, const char* filename, 
        void (*callback)(const void*,size_t)=nullptr);


} /* namespace module */
} /* namespace pgen */
