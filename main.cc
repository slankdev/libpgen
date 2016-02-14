

#include <pgen2.h>



int main() {

    pgen::ethernet pack;

    pack.ETH.src() = "11:22:33:44:55:66";
    pack.summary();
    pack.hex();

}

