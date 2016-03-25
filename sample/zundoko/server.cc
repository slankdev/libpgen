
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <pgen2.h>


void client_hanle(int sock){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof client_addr);
    socklen_t len = sizeof(client_addr);

    char buffer[10000];
    memset(buffer, 0, sizeof buffer);
    ssize_t recvlen = recvfrom(sock, buffer, sizeof buffer, 0, 
            (sockaddr*)&client_addr, &len);
    if (recvlen < 0) {
        perror("recv");
        close(sock);
        return;
    }
    
    printf("recv: %s \n", buffer);
    if (sendto(sock, buffer, strlen(buffer), 0, 
                (sockaddr*)&client_addr, sizeof client_addr) < 0) {
        perror("sendto");
        close(sock);
        return ;
    }
}


int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s DEVIDE PORT \n", argv[0]);
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ-1);
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl");
        close(sock);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr = ((sockaddr_in*)(&ifr.ifr_addr))->sin_addr;
    if (bind(sock, (struct sockaddr*)&addr, sizeof addr) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    printf("server info %s:%d/udp \n", inet_ntoa(addr.sin_addr), atoi(argv[2]));
    while (1)
        client_hanle(sock);
    close(sock);
}
