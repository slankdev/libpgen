
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "zundoko.h"


void send_zundoko(Socket& sock, struct sockaddr_in* addr) {
    char inputstr[1000];
    printf("input('z':send ZUN, 'd':send DOKO, ELSE:sned random): ");
    scanf("%s", inputstr);
    
    uint8_t buffer[1000];
    memset(buffer, 0, sizeof buffer);

    struct zundoko* zd = reinterpret_cast<zundoko*>(buffer);

    char* msg = reinterpret_cast<char*>(buffer+sizeof(zundoko));
    if (strcmp(inputstr, "z")==0) { /* ZUN */
        zd->type = htons(zundoko::ZUN);
        zd->message_length = htons(0);
    } else if (strcmp(inputstr, "d")==0) { /* DOKO */
        zd->type = htons(zundoko::DOKO);
        zd->message_length = htons(0);
    } else if (strcmp(inputstr, "zm") == 0) { /* ZUN add msg */
        printf("input msg: ");
        scanf("%s", msg);
        zd->type = htons(zundoko::ZUN);
        zd->message_length = htons(strlen(msg));
    } else if (strcmp(inputstr, "dm") == 0) { /* DOKO add msg */
        printf("input msg: ");
        scanf("%s", msg);
        zd->type = htons(zundoko::DOKO);
        zd->message_length = htons(strlen(msg));
    } else {
        srand(time(NULL));
        int r = rand() % 2;
        zd->type = htons(r==0 ? zundoko::ZUN : zundoko::DOKO);
        zd->message_length = htons(0);
    }
    
    sock.sendto(buffer, sizeof(struct zundoko)+ntohs(zd->message_length), 0, 
                (sockaddr*)addr, sizeof(sockaddr_in));
    printf("--------------sent--------------\n");
    printf("ZunDoko Type: %s \n", zd->type==zundoko::ZUN ? "ZUN" : "DOKO");
    printf("Message Len : %d \n", ntohs(zd->message_length));
    if (ntohs(zd->message_length) != 0) {
        printf("Message    : %s \n", msg);
    }
    printf("--------------------------------\n");

    socklen_t len = sizeof(sockaddr_in);
    memset(buffer, 0, sizeof buffer);
    ssize_t recvlen = sock.recvfrom(buffer, sizeof buffer, 0, 
            (sockaddr*)addr, &len);
}




int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s SERVER PORT \n", argv[0]);
        return -1;
    }
    
    Socket sock(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    printf("target server info %s:%d/udp \n", 
            inet_ntoa(addr.sin_addr), atoi(argv[2]));
    while (1) {
        send_zundoko(sock, &addr);
    }
}
