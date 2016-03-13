

#include <pgen2/io/stream.h>
#include <pgen2/types.h>
#include <pgen2/exception.h>
#include <pgen2/arch/arch.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>

#if defined(__PGEN_OSX)
#include <net/if_dl.h>
#include <net/bpf.h>
#endif


#include <fcntl.h>
#include <unistd.h>

#include <pgen2/arch/arch.h>

namespace pgen {


net_stream::net_stream() : _fd(-1) {}


net_stream::net_stream(const char* name, pgen::open_mode mode) : net_stream() {
    open(name, mode);
}


net_stream::~net_stream() {
    close();   
}


void net_stream::open_netif(const char* name) {
#if defined(__PGEN_OSX)
	const unsigned int one  = 1;

    for (int i = 0; i < 4; ++i) { 
        std::string buf = "/dev/bpf";
        buf += std::to_string(i);
        _fd = ::open(buf.c_str(), O_RDWR);
        if (_fd >= 0)
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
    // buffer size it too hj
	int bufsize = 4096;
    ioctl_c(BIOCSBLEN, &bufsize);

	// bind to device
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, name, IFNAMSIZ);
	ioctl_c(BIOCSETIF, &ifr);

	ioctl_c(BIOCPROMISC, NULL);     // set promisc
	ioctl_c(BIOCIMMEDIATE, &one);   //if recv packet then call read fast
	ioctl_c(BIOCSSEESENT, &one);    // set recv sendPacket
	ioctl_c(BIOCFLUSH, NULL);       // flush recv buffer
	ioctl_c(BIOCSHDRCMPLT, &one);   // no complite src macaddr
#elif defined(__PGEN_LINUX)
    
    printf("pgen::net_stream::open_netif: for linux is not implemented yet\n");
    exit(-1);

#else
    throw pgen::exception("pgen::net_stream::open_netif: unknown error");
#endif
}



void net_stream::write(const void* buf, size_t buflen) {
    ssize_t write_len = ::write(_fd, buf, buflen);
    if (write_len < 0) {
        throw pgen::exception("pgen::net_stream::write::write: ");
    }
}

size_t net_stream::read(void* buf, size_t buflen) {
    if (buflen != 4096) {
        throw pgen::exception("pgen::net_stream::read: [BUG!!!?] it is not OK, if buflen != 4096");
    }
    ssize_t read_len = ::read(_fd, buf, buflen);
    if (read_len < 0) {
        throw pgen::exception("pgen::net_stream::read::read: ");
    }
    return read_len;
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



/*
 * TODO
 * This function uses temporary stored memory, so this function
 * has potential that getting speed up.
 */
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
