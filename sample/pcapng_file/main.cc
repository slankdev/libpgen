
#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* out = "out.pcapng";
const char* in  = "in.pcapng";

using std::cout;
using std::endl;

#if 0
int main() {
    try {
        pgen::pcapng_stream pcapng;
        pcapng.open(in, pgen::open_mode::pcapng_read);
        while (1) {
            uint8_t buf[10000];
            try {
                size_t recvlen = pcapng.recv(buf, sizeof(buf));
                pgen::packet_type type = pgen::module::detect(buf, recvlen);
                printf("packet length = %zd \n", recvlen);
                pgen::hex(buf, recvlen);
            } catch (std::exception& e) {
                printf("recv exception: %s \n", e.what());
                break;
            }
        }
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
#else 
int main() {
    try {
        pgen::arp pack;
        pack.compile();
        
        pgen::pcapng_stream pcapng;
        pcapng.open(out, pgen::open_mode::pcapng_write);
        pcapng << pack;

    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
#endif


