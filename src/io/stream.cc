
#include <pgen2/io/stream.h>
#include <stdio.h>

namespace pgen {




stream::stream() {
    _offline_fd = NULL;
}
stream::~stream() {
    close();
}
pgen::open_mode stream::mode() const {
    return _mode;
}



void stream::open_file(const char* fname, const char* mode) {
    _offline_fd = fopen(fname, mode);
    if (_offline_fd == NULL) {
        throw pgen::exception("pgen::stream::open_file::fopen: ");
    }
}
void stream::write(const void* buf, size_t buflen) {
    size_t number_of_write = fwrite(buf, buflen, 1, _offline_fd);   
    if (number_of_write != 1) {
        throw pgen::exception("pgen::stream::open_file::fwirte: ");
    }
}
void stream::read(void* buf, size_t buflen) {
    size_t number_of_read = fread(buf, buflen, 1, _offline_fd);   
    if (number_of_read != 1) {
        throw pgen::exception("pgen::stream::open_file::fread: ");
    }
}



void stream::open(const char* name, pgen::open_mode mode) {
    struct pgen::pcap_file_header fhdr;
    switch (mode) {
        case pgen::open_mode::pcap_write:
            open_file(name, "wb");
            fhdr.magic = 0xa1b2c3d4;
            fhdr.version_major = 0x0002;
            fhdr.version_minor = 0x0004;
            fhdr.timezone = 0x0000;
            fhdr.sigfigs  = 0x0000;
            fhdr.snaplen  = 0x0000ffff;
            fhdr.linktype = 0x00000001;
            write(&fhdr, sizeof(fhdr));
            break;

        case pgen::open_mode::pcap_read:
            open_file(name, "rb");
            read(&fhdr, sizeof(fhdr));
            break;

        case pgen::open_mode::pcapng_write:
            throw pgen::exception("pgen::stream::open: pcap_write int't implemented\n");
            break;

        case pgen::open_mode::pcapng_read:
            throw pgen::exception("pgen::stream::open: pcap_read int't implemented\n");
            break;

        case pgen::open_mode::netif:
            throw pgen::exception("pgen::stream::open: netif int't implemented\n");
            break;

        default:
            throw pgen::exception("pgen::stream::open: unknown mode \n");
            break;
    }

}
void stream::close() {
    if (_offline_fd != NULL) 
        fclose(_offline_fd);
}



void stream::send(const void* buf, size_t buflen) {
    struct pgen::pcap_packet_headr phdr;
    phdr.timestamp_sec  = 0;
    phdr.timestamp_usec = 0;
    phdr.include_len    = buflen;
    phdr.original_len   = buflen;

    this->write(&phdr, sizeof(phdr));
    this->write(buf, buflen);
}
size_t stream::recv(void* buf, size_t buflen) {
    struct pgen::pcap_packet_headr phdr;
    this->read(&phdr, sizeof(phdr));
    
    if (buflen < phdr.original_len)
        throw pgen::exception("pgen::stream::recv: buffer length is too small");

    this->read(buf, phdr.original_len);
    return phdr.original_len;
}





} /* namespace pgen */
