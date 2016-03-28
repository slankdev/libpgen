
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

        pgen::net_stream net(dev, pgen::open_mode::netif);
        uint8_t buf[10000];

        while (1) {
            size_t recvlen = net.recv(buf, sizeof buf);

            pgen::packet_type type = pgen::module::detect(buf, recvlen);
            if (type == pgen::packet_type::udp) {
                pgen::udp udp(buf, recvlen);
                if (udp.UDP.src==pgen::udp::port::zundoko || udp.UDP.dst==pgen::udp::port::zundoko) {
                    pgen::zundoko pack(buf, recvlen);
                    if (pack.ZUNDOKO.type == pgen::zundoko::type::kiyoshi) {
                        printf("Success %s msg=\"%s\" \n", pack.IP.src.str().c_str(),
                                pack.ZUNDOKO.msg.c_str());
                    }
                }
            }
        }

        
        


#if 0
        pgen::pcapng_stream pcapng(in, pgen::open_mode::pcapng_read);
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
