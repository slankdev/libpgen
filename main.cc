

#include <pgen2.h>


int main() {

    pgen::core::ethernet pack;

    pack.ETH.src() = "11:22:33:44:55:66";
    pack.summary();

    // pgen::io::util::hex(pack.raw(), pack.length());
}

