
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"


void client_hanle(Socket& sock) {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof client_addr);
    socklen_t len = sizeof(client_addr);

    char buffer[10000];
    memset(buffer, 0, sizeof buffer);
    sock.recvfrom(buffer, sizeof buffer, 0, 
            (sockaddr*)&client_addr, &len);
    
    printf("recv: %s \n", buffer);
    sock.sendto(buffer, strlen(buffer), 0, 
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

    printf("server info %s:%d/udp \n", 
            inet_ntoa(addr.sin_addr), atoi(argv[2]));
    while (1)
        client_hanle(sock);
}


