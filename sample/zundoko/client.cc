
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
#include <time.h>
#include <pgen2.h>



void send_zundoko(int sock, struct sockaddr_in* addr) {
    char inputstr[1000];
    printf("input('z':send ZUN, 'd':send DOKO, ELSE:sned random): ");
    scanf("%s", inputstr);
    
    bool isZUN;
    if (strcmp(inputstr, "z")==0) { /* ZUN */
        isZUN = true;
    } else if (strcmp(inputstr, "d")==0) { /* DOKO */
        isZUN = false;
    } else {
        srand(time(NULL));
        int r = rand() % 2;
        isZUN = r==0 ? true : false;
    }
    
    
    char str[1000];
    if (isZUN)
        strcpy(str, "ZUN");
    else
        strcpy(str, "DOKO");
        
    if (sendto(sock, str, strlen(str), 0, 
                (sockaddr*)addr, sizeof(sockaddr_in)) < 0) {
        perror("sendto");
        close(sock);
    }

    socklen_t len = sizeof(sockaddr_in);
    memset(str, 0, sizeof str);
    ssize_t recvlen = recvfrom(sock, str, sizeof str, 0, 
            (sockaddr*)addr, &len);
    if (recvlen < 0) {
        perror("recv");
        close(sock);
        return;
    }
    printf("recv: %s \n", str);
}





int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s SERVER PORT \n", argv[0]);
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("target server info %s:%d/udp \n", inet_ntoa(addr.sin_addr), atoi(argv[2]));
    while (1) {
        send_zundoko(sock, &addr);
    }
    close(sock);
    return 0;
}
