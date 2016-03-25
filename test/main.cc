
/*
 * Checking pgen::udp_header::calc_checksum()
 */


#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "en0";
const char* out = "out.pcapng";
const char* in  = "in.pcapng";

using std::cout;
using std::endl;

int main() {
    try {

        pgen::pcapng_stream pcapng(in, pgen::open_mode::pcapng_read);
        uint8_t buf[10000];
        size_t recvlen = pcapng.recv(buf, sizeof buf);
        pgen::hex(buf, recvlen);
        printf("\n");

        pgen::udp pack(buf, recvlen);
        pack.UDP.summary(true);

        pack.UDP.check = 0;
        pack.UDP.check = pack.UDP.calc_checksum(pack.IP, pack.contents(),
                pack.length()-pack.header_length());
        pack.compile();
        pack.UDP.summary(true);

        pack.hex();
        // pcapng << pack;
        

    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
