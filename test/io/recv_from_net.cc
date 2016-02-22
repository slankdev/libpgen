

#include <pgen2.h>

int main() {
    try {
        pgen::net_stream s;
        s.open("en0", pgen::open_mode::netif);

        uint8_t buf[4096];
        size_t recvlen = s.recv(buf, sizeof(buf));
        
        printf("recv %zd bytes \n", recvlen);
        
    } catch (std::exception& e)  {
        printf("%s \n", e.what());
    }
}
