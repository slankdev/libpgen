
#pragma once


#include <stddef.h>
#include <stdint.h>
// #include <>


namespace pgen {

    
struct bpf_header {
    uint64_t    timestamp;
	uint32_t	caplen;
	uint32_t	datalen;
	uint16_t	hdrlen;	
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
