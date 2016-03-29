

#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* in  = "in.pcapng";

#define ZDPORT 9988
using std::cout;
using std::endl;

int main() {
    try 
    {
        pgen::pcapng_stream pcapng(in, pgen::open_mode::pcapng_read);

        while (1) {
            uint8_t buf[10000];
            size_t recvlen = pcapng.recv(buf, sizeof buf);

            pgen::packet_type type = pgen::module::detect(buf, recvlen);
            if (type == pgen::packet_type::udp) {
                pgen::udp udp(buf, recvlen);
                if (udp.UDP.src==ZDPORT || udp.UDP.dst==ZDPORT) {
                    pgen::zundoko pack(buf, recvlen);
                    if (pack.ZUNDOKO.type == pgen::zundoko::type::kiyoshi) {
                        printf("Success %s msg=\"%s\" \n", pack.IP.dst.str().c_str(),
                                pack.ZUNDOKO.msg.c_str());
                    } else {
                    }
                }
            }
        }

        
    }
    catch (std::exception& e)
    {
        printf("%s \n", e.what());
    }
}

