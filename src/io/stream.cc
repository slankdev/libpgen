
#include <pgen2/io/stream.h>
#include <stdio.h>

namespace pgen {
namespace io   {

    
size_t stream::send(const void* buffer, size_t bufferlen) {
    if (buffer == NULL) return 1;
    printf("not implemented yet \n");
    return bufferlen;   
}

size_t stream::recv(void* buffer, size_t bufferlen) {
    if (buffer == NULL) return 1;
    printf("not implemented yet \n");
    return bufferlen;   
}


} /* namespace io   */
} /* namespace pgen */
