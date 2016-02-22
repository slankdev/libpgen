
#include <pgen2.h>

const char* rfile = "in.pcap";
const char* wfile = "out.pcap";
const char* dev = "en0";


void recv_net();



int main() {
    recv_net();
}



void recv_net() {
    try {
        pgen::net_stream r;
        r.open(dev, pgen::open_mode::netif);

        int c= 0;
        while (1) {

            uint8_t buf[4096];
            size_t recvlen = r.recv(buf, sizeof(buf));
            
            uint8_t* p = buf;
            // printf("%02x ", p[12]);
            // printf("%02x ", p[13]);
            if (p[12]==0x86 && p[13]==0xdd){
                c++;
                printf("%5d   recv IPv6\n", c);
            }

        }
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}




