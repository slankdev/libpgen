
/*
 * Checking pgen::udp_header::calc_checksum()
 */


#include <iostream>
#include <pgen2/core.h>
#include <pgen2/io.h>
#include <pgen2/module.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "wlxcce1d51517b5";
const char* out = "out.pcapng";
const char* in  = "in.pcapng";

using std::cout;
using std::endl;



int main() {
    try {

        pgen::net_stream stream(dev, pgen::open_mode::netif);
        // pgen::pcapng_stream stream(in, pgen::open_mode::pcapng_read);
        uint8_t buf[10000];

        while (1) {
            size_t recvlen = stream.recv(buf, sizeof buf);

            if (pgen::module::detect_ipv6(buf, recvlen)) {
                printf("filterd \n");
                pgen::ipv6 pack;
                pack.analyze(buf, recvlen);
                pack.IP.summary(true);
            } else {
                // printf("others \n");
            }
        }


    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
