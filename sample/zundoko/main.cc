

#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "en0";
const char* out = "out.pcapng";
const char* in  = "in.pcapng";
#define ZDPORT 9988
using std::cout;
using std::endl;

int main() {
    try 
    {

        pgen::net_stream net(in, pgen::open_mode::netif);
        uint8_t buf[10000];

        while (1) {
            size_t recvlen = net.recv(buf, sizeof buf);

            pgen::packet_type type = pgen::module::detect(buf, recvlen);
            if (type == pgen::packet_type::udp) {
                pgen::udp udp(buf, recvlen);
                if (udp.UDP.src==ZDPORT || udp.UDP.dst==ZDPORT) {
                    pgen::zundoko pack(buf, recvlen);
                    if (pack.ZUNDOKO.type == pgen::zundoko::type::kiyoshi) {
                        printf("Success %s msg=\"%s\" \n", pack.IP.dst.str().c_str(),
                                pack.ZUNDOKO.msg.c_str());
                    } else {
                        printf("other type \n");
    
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

