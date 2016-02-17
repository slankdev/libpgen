

#include <pgen2.h>
#include <iostream>
#include <exception>

int main() {
#ifdef __PGEN_LINUX
    printf("is linux \n");
#endif
#ifdef __PGEN_MAXOS
    printf("is osx \n");
#endif
    // pgen::ethernet pack;
    // pack.compile();
    // pack.hex();
}
    
