
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

struct pcap_fhdr{
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t timezone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};

} /* namespace pgen */
