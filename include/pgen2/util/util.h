

#pragma once
#include <stddef.h>


namespace pgen {
namespace util {

void hex(const void* buffer, size_t bufferlen);

size_t send_to_netif(int fd, const void* buffer, size_t bufferlen);
// size_t send_to_pcap(FILE* fd, const void* buffer, size_t bufferlen);
// size_t send_to_pcapng(FILE* fd, const void* buffer, size_t bufferlen);
//
// size_t recv_from_netif(int fd, void* buffer, size_t bufferlen);
// size_t recv_from_pcap(FILE* fd, void* buffer, size_t bufferlen);
// size_t recv_from_pcapng(FILE* fd, void* buffer, size_t bufferlen);



} /* namespace util */
} /* namespace pgen */
