

#include <pgen2.h>
#include <pgen2/exception.h>
#include <iostream>
#include <exception>

int main() {
    pgen::ethernet pack;
    pack.compile();
    pack.hex();
}
    
