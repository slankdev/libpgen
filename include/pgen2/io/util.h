

#pragma once
#include <stddef.h>
#include <pgen2/types.h>


namespace pgen {


namespace util {

int  open_netif(const char* ifname);
void close_netif(int fd);
size_t send_to_netif(int fd, const void* buffer, size_t bufferlen);
size_t recv_from_netif(int fd, void* buffer, size_t bufferlen);

FILE*  open_pcap(const char* filename, pgen::open_mode mode);
size_t send_to_pcap(FILE* fd, const void* buffer, size_t bufferlen);

} /* namespace util */



// FILE*  open_pcapng(const char* filename, mode);

// void close_pcap(FILE* fd);
// void close_pcapng(FILE* fd);

// size_t send_to_pcapng(FILE* fd, const void* buffer, size_t bufferlen);

// size_t recv_from_pcap(FILE* fd, void* buffer, size_t bufferlen);
// size_t recv_from_pcapng(FILE* fd, void* buffer, size_t bufferlen);


void hex(const void* buffer, size_t bufferlen);


} /* namespace pgen */
