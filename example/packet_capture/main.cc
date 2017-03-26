
#include <pgen2.h>
const char* dev = "lo";
int main() {
    try {
        pgen::net_stream handle;
        handle.open(dev, pgen::open_mode::netif);

        while (1) {
            uint8_t buf[4096];
            size_t recv_len = handle.recv(buf, sizeof(buf));

            pgen::hex(buf, recv_len);
        }
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
