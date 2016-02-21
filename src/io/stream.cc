
#include <pgen2/io/stream.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>


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





net_stream::net_stream() : _fd(-1) {}

net_stream::~net_stream() {
    close();   
}
pgen::open_mode net_stream::mode() const {
    return _mode;   
}


void net_stream::open_netif(const char* name) {
	const unsigned int one  = 1;

	for (int i = 0; i < 4; i++) { 
        std::string buf = "/dev/bpf";
        buf += std::to_string(i);
		_fd = ::open(buf.c_str(), O_RDWR);
        if (_fd > 0)
			break;
	}
    if (_fd < 0)
        throw pgen::exception("pgen::net_stream::open_netif::open: ");
	

    /*
     * TODO 
     * This ioctl task is hard-coding, 
     * about buffer size is always 4096 byte.
     */
    // set bufsize   
	int bufsize = 4096;
    ioctl_c(BIOCSBLEN, &bufsize);

	// bind to device
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, name, IFNAMSIZ);
	ioctl_c(BIOCSETIF, &ifr);


	// set promisc
	ioctl_c(BIOCPROMISC, NULL);

	//if recv packet then call read fast
	ioctl_c(BIOCIMMEDIATE, &one);

	// set recv sendPacket 
	ioctl_c(BIOCSSEESENT, &one);

	// flush recv buffer
	ioctl_c(BIOCFLUSH, NULL);

	// no complite src macaddr
	ioctl_c(BIOCSHDRCMPLT, &one);
}


void net_stream::write(const void* buf, size_t buflen) {
    int write_len = ::write(_fd, buf, buflen);
    if (write_len < 0) {
        throw pgen::exception("pgen::net_stream::write::write: ");
    }
}

size_t net_stream::read(void* buf, size_t buflen) {
    if (buflen != 4096) {
        throw pgen::exception("pgen::net_stream::read: [BUG!!!?] it is not OK, if buflen != 4096");
    }
    int read_len = ::read(_fd, buf, buflen);
    if (read_len < 0) {
        throw pgen::exception("pgen::net_stream::read::read: ");
    }
    return read_len;
}

void net_stream::ioctl(unsigned long l, void* p) {
    int res = ::ioctl(_fd, l, p);
    if (res < 0) {
        throw pgen::exception("pgen::net_stream::ioctl: ");
    }
}
void net_stream::ioctl_c(unsigned long l, const void* p) {
    int res = ::ioctl(_fd, l, p);
    if (res < 0) {
        throw pgen::exception("pgen::net_stream::ioctl: ");
    }
}


void net_stream::open(const char* name, pgen::open_mode mode) {
    if (mode == pgen::open_mode::netif) {
        this->open_netif(name);
    } else {
        throw pgen::exception("pgen::net_stream::open: unknown mode");
    }
    
}

void net_stream::close() {
    if (_fd >= 0) 
        ::close(_fd);
}

size_t net_stream::send(const void* buf, size_t buflen) {
    this->write(buf, buflen);
    return buflen;
}

size_t net_stream::recv(void* buf, size_t buflen) {
    uint8_t b[4096];
    this->read(b, sizeof(b));
    struct pgen::bpf_header* bpfh = (struct pgen::bpf_header*)b;
    if (bpfh->caplen > buflen) {
        throw pgen::exception("pgen::net_stream::recv: buflen is too small");
    }
    memcpy(buf, b+bpfh->hdrlen, bpfh->caplen);
    return bpfh->caplen;
}



} /* namespace pgen */
