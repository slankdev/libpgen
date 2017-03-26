

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>
#include <pgen/io/file_stream.h>


/* for impl */
#include <pgen/io/pcap_stream.h>
#include <pgen/io/pcapng_stream.h>
#include <pgen/exception.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



namespace pgen {





struct pcap_file_header {
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t timezone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};



class pcap_stream : public file_stream {
    private:
        struct pcap_file_header file_header;
    public:

        pcap_stream();
        pcap_stream(const char* name, pgen::open_mode mode);

        void open(const char* name, pgen::open_mode _mode);
        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

};


/*
 * Function Impl
 */


inline pcap_stream::pcap_stream() {}
inline pcap_stream::pcap_stream(const char* name, pgen::open_mode mode)
            : pcap_stream() {
    open(name, mode);
}


inline void pcap_stream::open(const char* name, pgen::open_mode mode) {
    switch (mode) {
        case pgen::open_mode::pcap_write:
        {
            fopen(name, "wb");
            file_header.magic = 0xa1b2c3d4;
            file_header.version_major = 0x0002;
            file_header.version_minor = 0x0004;
            file_header.timezone = 0x0000;
            file_header.sigfigs  = 0x0000;
            file_header.snaplen  = 0x0000ffff;
            file_header.linktype = 0x00000001;
            write(&file_header, sizeof(file_header));
            break;
        }
        case pgen::open_mode::pcap_read:
        {
            fopen(name, "rb");
            read(&file_header, sizeof(file_header));
            break;
        }
        default:
        {
            throw pgen::exception(
                    "pgen::pcap_stream::open: unknown mode \n");
            break;
        }
    }
}




struct pcap_packet_header {
    uint32_t timestamp_sec;
    uint32_t timestamp_usec;
    uint32_t include_len;
    uint32_t original_len;
};


inline void pcap_stream::send(const void* buf, size_t buflen) {

    struct pcap_packet_header ph;
    ph.timestamp_sec  = 0;
    ph.timestamp_usec = 0;
    ph.include_len    = buflen;
    ph.original_len   = buflen;

    this->write(&ph, sizeof(ph));
    this->write(buf, buflen);

}


inline size_t pcap_stream::recv(void* buf, size_t buflen) {

    struct pcap_packet_header ph;
    this->read(&ph, sizeof(ph));

    if (buflen < ph.original_len) {
        throw pgen::exception(
                "pgen::pcap_stream::recv: buffer length is too small");
    }

    this->read(buf, ph.original_len);
    return ph.original_len;
}




} /* namespace pgen */


