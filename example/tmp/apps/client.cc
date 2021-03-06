
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <slankdev.h>
#include "tmp.h"

#define BUFSIZE 1000

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s SERVERIP USERID \n", argv[0]);
        return -1;
    }

    slankdev::safe_intfd fd;
    fd.socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERV_PORT);
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    printf("[connect] %s:%d/udp \n", 
            inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

    for (int seqence=0; ; seqence++) {
        char msg[BUFSIZE-sizeof(tmp_header)];
        printf("msg>> ");
        fgets(msg, sizeof msg, stdin);
        msg[strlen(msg)-1] = '\0';
        size_t msglen = strlen(msg)+1;
        size_t totallen = sizeof(struct tmp_header)+msglen;

        uint8_t buf[BUFSIZE];
        struct tmp_header* tmph = (struct tmp_header*)buf;
        tmph->id = htonl(strtol(argv[2], NULL, 16));
        tmph->seq= htons(seqence);
        tmph->msg_len = htons(msglen);
        memcpy(buf+sizeof(tmp_header), msg, msglen);

        fd.sendto(buf, sizeof(tmp_header)+msglen, 
                0, (struct sockaddr*)&sin, sizeof sin);
    }
}
