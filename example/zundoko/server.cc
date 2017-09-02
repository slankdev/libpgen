
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zundoko.h"

enum class zd_state {
    none,
    z,
    zz,
    zzz,
    zzzz,
};
zd_state state;


void client_hanle(Socket& sock) {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof client_addr);
    socklen_t len = sizeof(client_addr);

    char buffer[10000];
    memset(buffer, 0, sizeof buffer);
    ssize_t recvlen = sock.recvfrom(buffer, sizeof buffer, 0, 
            (sockaddr*)&client_addr, &len);
    
    struct zundoko* zd = reinterpret_cast<zundoko*>(buffer);
    uint16_t type = ntohs(zd->type);
    printf("Recv type=%s msglen=%d ", 
            type==zundoko::ZUN ? "ZUN" : 
            type==zundoko::DOKO ? "DOKO" : 
            "UNKNOWN", ntohs(zd->message_length));

    if (ntohs(zd->message_length) != 0) {
        printf("msg=\"%s\" ", buffer+sizeof(zundoko));
    }

    switch (state) {
        case zd_state::none:
        {
            if (type == zundoko::ZUN)
                state = zd_state::z;
            else
                state = zd_state::none;
            break;
        }
        case zd_state::z:
        {
            if (type == zundoko::ZUN)
                state = zd_state::zz;
            else
                state = zd_state::none;
            break;
        }
        case zd_state::zz:
        {
            if (type == zundoko::ZUN)
                state = zd_state::zzz;
            else
                state = zd_state::none;
            break;
        }
        case zd_state::zzz:
        {
            if (type == zundoko::ZUN)
                state = zd_state::zzzz;
            else
                state = zd_state::none;
            break;
        }
        case zd_state::zzzz:
        {
            if (type == zundoko::DOKO) {
                zd->type = htons(zundoko::KIYOSHI);
                printf("Succsess ");
            }
            state = zd_state::none;
            break;
        }
        default:
        {
            printf("unknown state \n");
            exit(-1);
            break;
        }
    }
    switch (state) {
        case zd_state::none:
        {
            printf("state=none\n");
            break;
        }
        case zd_state::z:
        {
            printf("state=z\n");
            break;
        }
        case zd_state::zz:
        {
            printf("state=zz\n");
            break;
        }
        case zd_state::zzz:
        {
            printf("state=zzz\n");
            break;
        }
        case zd_state::zzzz:
        {
            printf("state=zzzz\n");
            break;
        }
        default:
        {
            printf("unknown state \n");
            exit(-1);
            break;
        }
    }

    sock.sendto(buffer, recvlen, 0, 
                (sockaddr*)&client_addr, sizeof client_addr);
}


int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s DEVIDE PORT\n", argv[0]);
        return -1;
    }

    Socket sock(AF_INET, SOCK_DGRAM, 0);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ-1);
    sock.ioctl(SIOCGIFADDR, &ifr);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr = ((sockaddr_in*)(&ifr.ifr_addr))->sin_addr;
    sock.bind((struct sockaddr*)&addr, sizeof addr);

    state = zd_state::none;
    printf("server info %s:%d/udp \n", 
            inet_ntoa(addr.sin_addr), atoi(argv[2]));
    while (1)
        client_hanle(sock);
}


