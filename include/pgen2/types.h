
#pragma once


#include <stddef.h>
#include <stdint.h>

namespace pgen {

enum class open_mode {
    pcap_read,
    pcap_write,
    pcapng_read,
    pcapng_write,
    netif
};

struct pcap_file_header {
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t timezone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};


struct pcap_packet_headr {
    uint32_t timestamp_sec;
    uint32_t timestamp_usec;
    uint32_t include_len;
    uint32_t original_len;
};




} /* namespace pgen */
