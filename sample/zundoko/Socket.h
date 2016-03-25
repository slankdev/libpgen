
#pragma once

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


class Socket {
    private:
        int fd;

    public:
        
        Socket() {}
        Socket(int domain, int type, int protocol) {
            socket(domain, type, protocol);   
        }
        ~Socket() {
            close();
        }

        void socket(int domain, int type, int protocol) {
            fd = ::socket(domain, type, protocol);
            if (fd < 0) {
                perror("socket");
                exit(-1);
            }
        }
        void close() {
            if (fd >= 0) 
                ::close(fd);
        }
        void bind(const struct sockaddr* addr, socklen_t addrlen) {
            int res = ::bind(fd, addr, addrlen);
            if (res < 0) {
                perror("bind");
                exit(-1);
            }
        }
        void ioctl(unsigned long request, void* arg) {
            int res = ::ioctl(fd, request, arg);
            if (res < 0) {
                perror("ioctl");
                exit(-1);
            }
        }
        void sendto(const void* message, size_t length, int flag, 
                const struct sockaddr* dst_addr, socklen_t dst_len) {
            ssize_t res = ::sendto(fd, message, length, 
                    flag, dst_addr, dst_len);
            if (res < 0) {
                perror("sendto");
                exit(-1);
            }
        }
        ssize_t recvfrom(void* message, size_t length, int flag, 
                struct sockaddr* dst_addr, socklen_t* dst_len) {
            ssize_t res = ::recvfrom(fd, message, length, 
                    flag, dst_addr, dst_len);
            if (res < 0) {
                perror("recvfrom");
                exit(-1);
            }
            return res;
        }

    
};
