
#include <pgen2.h>
using pgen::arp;
using pgen::net_stream;
using pgen::module::detect;
using pgen::open_mode;
using pgen::packet_type;

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s interface tergetIP\n", argv[0]);
        return -1;
    }

    try {
        arp send_pack;   
        send_pack.ETH.src.setbydev(argv[1]);
        send_pack.ETH.dst.setbcast();
        send_pack.ARP.operation = arp::operation::request;
        send_pack.ARP.hwsrc = send_pack.ETH.src;
        send_pack.ARP.psrc.setbydev(argv[1]);
        send_pack.ARP.hwdst = send_pack.ETH.dst;
        send_pack.ARP.pdst = argv[2];

        net_stream net(argv[1], open_mode::netif);

        printf("ARPING %s from %s %s\n", argv[2], 
                send_pack.ETH.src.str().c_str(), argv[1]);
        for (int count=0; count<5; ) {
            send_pack.compile();
            net << send_pack;

            uint8_t recvbuf[10000];
            size_t recvlen = net.recv(recvbuf, sizeof recvbuf);

            if (detect(recvbuf, recvlen) == pgen::packet_type::arp) {
                arp recv_pack(recvbuf, recvlen);
                if (recv_pack.ARP.operation == arp::operation::reply) {
                    recv_pack.ARP.summary();
                    sleep(1);
                    count++;
                }
            }
        }
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
