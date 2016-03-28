

#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "wlan0";
const char* out = "out.pcapng";
const char* in  = "in.pcapng";

using std::cout;
using std::endl;

int main() {
    try 
    {

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
                        printf("Success %s msg=\"%s\" \n", pack.IP.dst.str().c_str(),
                                pack.ZUNDOKO.msg.c_str());
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

