
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <slankdev.h>
#include "tmp.h"

#define BUFSIZE 10000
#define DEVICE "DEVICENAME"

void handle_client(slankdev::safe_intfd& fd)
{
    struct sockaddr_in client;
    socklen_t client_len = sizeof client;

    uint8_t buf[BUFSIZE];
    size_t recvlen = fd.recvfrom(buf, sizeof buf, 0, 
            (struct sockaddr*)&client, &client_len);
    
    if (recvlen < sizeof(struct tmp_header)) {
        slankdev::hexdump("isn't tmp packet", buf, recvlen);
        return;
    }
    struct tmp_header* tmph = (struct tmp_header*)buf;
    if (recvlen < sizeof(struct tmp_header)+ntohs(tmph->msg_len)) {
        slankdev::hexdump("malformed packet", buf, recvlen);
        return;
    }

    printf("--------------------------\n");
    printf("ID    : 0x%x\n", htonl(tmph->id));
    printf("SeqNum: %d\n", htons(tmph->seq));
    printf("MsgLen: %d\n", htons(tmph->msg_len));
    printf("Msg   : %s\n", buf+sizeof(tmp_header));
    printf("--------------------------\n");
}


int main()
{
    slankdev::safe_intfd fd;
    fd.socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ-1);
    fd.ioctl(SIOCGIFADDR, &ifr);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);
    sin.sin_addr = ((sockaddr_in*)(&ifr.ifr_addr))->sin_addr;
    fd.bind((struct sockaddr*)&sin, sizeof sin);
    printf("[server] %s:%d/udp \n", 
            inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

    for (;;)
        handle_client(fd);
}


