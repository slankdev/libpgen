
/*
 * Checking pgen::udp_header::calc_checksum()
 */


#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "lo";
const char* out = "out.pcapng";
const char* in  = "in.pcapng";

using std::cout;
using std::endl;

int main() {
    try {

        // pgen::net_stream stream(dev, pgen::open_mode::netif);
        pgen::pcapng_stream stream(in, pgen::open_mode::pcapng_read);
        uint8_t buf[10000];

        while (1) {
            size_t recvlen = stream.recv(buf, sizeof buf);

            pgen::ptype_l3 type = pgen::module::detect_l3(buf, recvlen);
            if (type == pgen::ptype_l3::ipv6) {
                printf("ip v6 \n");
            } else {
                printf("others \n");
            }
        }


    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
