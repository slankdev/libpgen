

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>
#include <pgen2/io/file_stream.h>




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
        void close() noexcept;
        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

};


} /* namespace pgen */


