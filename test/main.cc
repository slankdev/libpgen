
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
        pgen::zundoko_header z;
        z.clear();
        z.msg = "test";
        z.msg_len = z.msg.length();
        z.summary(true);

        uint8_t buf[1000];
        z.write(buf, sizeof buf);
        pgen::hex(buf, z.length());

        pgen::zundoko_header d;
        d.read(buf, z.length());
        d.summary(true);
        d.write(buf, sizeof buf);
        pgen::hex(buf, d.length());


#if 0
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
#endif       

    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
