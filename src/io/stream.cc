
#include <pgen2/io/stream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



namespace pgen {


void base_stream::print() {
    printf("test \n");
}




pcap_stream::pcap_stream() : _fd(NULL) {
}
pcap_stream::~pcap_stream() {
    close();
}
pgen::open_mode pcap_stream::mode() const {
    return _mode;
}



void pcap_stream::fopen(const char* name, const char* mode){
    _fd = ::fopen(name, mode);
    if (_fd == NULL)
        throw pgen::exception("pgen::pcap_stream::fopen: ");
}
void pcap_stream::fwrite(const void* buf, size_t buflen) {
    size_t number_of_write = ::fwrite(buf, buflen, 1, _fd);   
    if (number_of_write != 1) {
        throw pgen::exception("pgen::pcap_stream::open_file::fwirte: ");
    }
}
size_t pcap_stream::fread(void* buf, size_t buflen) {
    size_t number_of_read = ::fread(buf, buflen, 1, _fd);   
    if (number_of_read != 1) {
        throw pgen::exception("pgen::pcap_stream::open_file::fread: ");
    }
    return buflen;
}



void pcap_stream::open(const char* name, pgen::open_mode mode) {
    struct pgen::pcap_file_header fhdr;
    switch (mode) {
        case pgen::open_mode::pcap_write:
            fopen(name, "wb");
            fhdr.magic = 0xa1b2c3d4;
            fhdr.version_major = 0x0002;
            fhdr.version_minor = 0x0004;
            fhdr.timezone = 0x0000;
            fhdr.sigfigs  = 0x0000;
            fhdr.snaplen  = 0x0000ffff;
            fhdr.linktype = 0x00000001;
            fwrite(&fhdr, sizeof(fhdr));
            break;

        case pgen::open_mode::pcap_read:
            fopen(name, "rb");
            fread(&fhdr, sizeof(fhdr));
            break;

        default:
            throw pgen::exception("pgen::pcap_stream::open: unknown mode \n");
            break;
    }

}
void pcap_stream::close() {
    if (_fd != NULL) 
        fclose(_fd);
}



size_t pcap_stream::send(const void* buf, size_t buflen) {
   
    struct pgen::pcap_packet_headr phdr;
    phdr.timestamp_sec  = 0;
    phdr.timestamp_usec = 0;
    phdr.include_len    = buflen;
    phdr.original_len   = buflen;

    this->fwrite(&phdr, sizeof(phdr));
    this->fwrite(buf, buflen);

    return buflen;
}
size_t pcap_stream::recv(void* buf, size_t buflen) {
    
    struct pgen::pcap_packet_headr phdr;
    this->fread(&phdr, sizeof(phdr));
    
    if (buflen < phdr.original_len) {
        throw pgen::exception("pgen::pcap_stream::recv: buffer length is too small");
    }

    this->fread(buf, phdr.original_len);
    return phdr.original_len;
}



bool pcap_stream::eof() const {

    uint8_t c;
    int res = ::fread(&c, 1, 1, _fd);
    if (res != 1 && feof(_fd)) {
        return true;
    } else {
        fseek(_fd, -1, SEEK_CUR);
        return false;
    }
}


void pcap_stream::flush() const {
    fflush(_fd);
}





} /* namespace pgen */
