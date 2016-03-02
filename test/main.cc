
#include <pgen2.h>

const char* dev  = "en0";
const char* file = "test/in2.pcap";

void func(const void* buffer, size_t bufferlen) {
    pgen::hex(buffer, bufferlen);
    pgen::ipv4 pack(buffer, bufferlen);
    pack.compile();
    pack.hex();
    printf("total length is  %d \n", pack.IP.tot_len);
    exit(-1);
}


int main() {
    pgen::ipv4 pack;
    int count = pgen::module::check(&pack, file, func);
    printf("analyze %d packs \n", count);
}

