


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
        
        pgen::ipv4_header ip;
        ip.clear();

        pgen::udp_header u;
        u.src = 80;
        u.dst = 443;
        u.len = 0;
        u.check = 0xffff;

        // uint8_t buf[1000];
        // u.write(buf, sizeof buf);

        ip.summary();              
        cout << u.calc_checksum(ip, NULL, 0);

    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
