

#include <pgen2.h>
#include <time.h>
#include "tmp.h"
const char* in  = "in.pcap";

static const char* getnow()
{
    static char str[32];
    time_t now = time(NULL);
    struct tm* p = localtime(&now);
    sprintf(str, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
    return str;
}

int main()
{
    pgen::pcap_stream packet_stream(in, pgen::open_mode::pcap_read);
    while (1) {
        uint8_t buf[10000];
        size_t recvlen;
        try {
            recvlen = packet_stream.recv(buf, sizeof buf);
        } catch (std::exception& e) {
            return 0;
        }

        if (is_tmp_packet(buf, recvlen)) {
            tmp pack(buf, recvlen);
            printf("[%s] 0x%04x: %s\n", getnow(), 
                    pack.TMP.id, pack.TMP.msg.c_str());
        }
    }
}

