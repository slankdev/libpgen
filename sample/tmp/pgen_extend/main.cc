

#include <pgen2.h>
#include <slankdev.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tmp.h"
const uint32_t filtered_id = 0x1234;
const char* dev  = "lo";
const char* file = "out.log";

static const char* getnow()
{
    static char str[32];
    time_t now = time(NULL);
    struct tm* p = localtime(&now);
    sprintf(str, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
    return str;
}

void handle_packet(tmp& pack, slankdev::unsafe_intfd& log)
{
    if (pack.TMP.id = filtered_id) {
        char str[1000];
        sprintf(str, "[%s] 0x%04x: %s\n", getnow(), pack.TMP.id, pack.TMP.msg.c_str());
        log.write(str, strlen(str));
    }
    pack.TMP.summary();
}


int main()
{
    pgen::net_stream net(dev, pgen::open_mode::netif);
    slankdev::unsafe_intfd log;
    log.open(file, O_WRONLY);

    while (1) {
        uint8_t buf[10000];
        size_t recvlen = net.recv(buf, sizeof buf);
        if (is_tmp_packet(buf, recvlen)) {
            tmp pack(buf, recvlen);
            handle_packet(pack, log);
        }
    }
}

