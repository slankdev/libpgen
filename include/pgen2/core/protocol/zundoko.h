
#if 0

/* Func Impl */

#pragma once
#include <arpa/inet.h>
#include <pgen2/exception.h>
#include <pgen2/core/protocol/udp.h>


namespace pgen {


inline static void print_type(uint16_t type) {
    switch (type) {
        case 0:
            printf("zun");
            break;
        case 1:
            printf("doko");
            break;
        case 2:
            printf("kiyoshi");
            break;
        default:
            printf("unknown");
            break;
    }
}


inline void zundoko_header::clear() {
    type = 0;
    msg_len = 0;
    msg = "";
}


inline void zundoko_header::summary(bool moreinfo) const {
    printf("ZUNDOKO [type=");
    print_type(type);
    printf("]\n");

    if (moreinfo) {
        printf(" - type   : ");
        print_type(type);
        printf("\n");
        printf(" - msg_len: %d \n", msg_len);
        printf(" - msg    : %s \n", msg.c_str());
    }
}


struct zundoko_struct {
    uint16_t type;
    uint16_t msg_len;
};


inline void zundoko_header::write(void* buffer, size_t buffer_len) const {
    if (buffer_len < min_length) {
        throw pgen::exception("pgen::zundoko_header::write: buflen is too small");
    }

    uint8_t* buffer_point =
        reinterpret_cast<uint8_t*>(buffer);

    struct zundoko_struct* zd_head =
        reinterpret_cast<zundoko_struct*>(buffer_point);
    zd_head->type    = htons(type);
    zd_head->msg_len = htons(msg_len);

    char* message =
        reinterpret_cast<char*>(buffer_point+sizeof(zundoko_struct));
    memcpy(message, msg.c_str(), msg_len);
}


inline void zundoko_header::read(const void* buffer, size_t buffer_len) {
    if (buffer_len < min_length) {
        throw pgen::exception("pgen::zundoko_header::read: buflen is too small");
    }

    const uint8_t* buffer_point =
        reinterpret_cast<const uint8_t*>(buffer);

    const struct zundoko_struct* zd_head =
        reinterpret_cast<const zundoko_struct*>(buffer_point);
    type    = ntohs(zd_head->type   );
    msg_len = ntohs(zd_head->msg_len);

    const char* message =
        reinterpret_cast<const char*>(buffer_point+sizeof(zundoko_struct));
    if (msg_len > 0)
        msg = message;
}


inline size_t zundoko_header::length() const {
    return sizeof(zundoko_struct)+msg_len;
}






inline void zundoko::init_headers() {
    headers = {&ETH, &IP, &UDP, &ZUNDOKO};
}


inline zundoko::zundoko() {
    clear();
    init_headers();
}


inline zundoko::zundoko(const void* buffer, size_t bufferlen) : zundoko() {
    analyze(buffer, bufferlen);
}


inline zundoko::zundoko(const pgen::zundoko& rhs) : zundoko() {
    ZUNDOKO = rhs.ZUNDOKO;
    UDP     = rhs.UDP    ;
    IP      = rhs.IP     ;
    ETH     = rhs.ETH    ;
    init_headers();
}

inline void zundoko::clear() {
    ETH.clear();
    ETH.type = ethernet::type::ip;
    IP.clear();
    IP.protocol = pgen::ipv4::protocol::udp;
    IP.tot_len = IP.length() + UDP.length() + ZUNDOKO.length();
    UDP.clear();
    UDP.src = pgen::udp::port::zundoko;
    UDP.dst = pgen::udp::port::zundoko;
    UDP.len = UDP.length() + ZUNDOKO.length();
}


} /* namespace pgen */

#endif
