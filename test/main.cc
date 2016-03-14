


#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "en0";
const char* out = "out.pcap";


void callback(const void* buffer, size_t bufferlen) {
    pgen::packet_type t = pgen::module::detect(buffer, bufferlen); 
    if (t == pgen::packet_type::arp) {
        pgen::arp pack(buffer, bufferlen);
        if (pack.ARP.operation == pgen::arp::operation::request) {
            pgen::hex(buffer, bufferlen);
            // pack.ARP.summary(true);
            exit(1);
        }
    }
}


int main() {
    try {

        pgen::net_stream net(dev, pgen::open_mode::netif);
        while (1) {

            uint8_t buf[10000];
            size_t recvlen = net.recv(buf, sizeof buf);

            callback(buf, recvlen);
        }


    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}



