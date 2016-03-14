

#include <pgen2/io/stream.h>
#include <pgen2/exception.h>
#include <pgen2/arch/arch.h>

#include <string>
#include <vector>
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


void net_stream::open_netif(const char* name, size_t buffer_size) {
#if defined(__PGEN_OSX)

    for (int i = 0; i < 4; ++i) { 
        std::string buf = "/dev/bpf";
        buf += std::to_string(i);
        _fd = ::open(buf.c_str(), O_RDWR);
        if (_fd >= 0) break;
    }
    if (_fd < 0)
        throw pgen::exception("pgen::net_stream::open_netif::open: ");
	
    if (buffer_size == 0) {
        ioctl(BIOCGBLEN, &buffer_size);
    }

    _buffer.resize(buffer_size);
    ioctl(BIOCSBLEN, &buffer_size);

	// bind to device
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, name, IFNAMSIZ);
	ioctl(BIOCSETIF, &ifr);

    /* other config */
    unsigned int one = 1;
	ioctl(BIOCPROMISC, NULL);     // set promisc
	ioctl(BIOCIMMEDIATE, &one);   //if recv packet then call read fast
	ioctl(BIOCSSEESENT, &one);    // set recv sendPacket
	ioctl(BIOCFLUSH, NULL);       // flush recv buffer
	ioctl(BIOCSHDRCMPLT, &one);   // no complite src macaddr
    _buffer_point = NULL;           // init _buffer_point
#elif defined(__PGEN_LINUX)
    
    printf("pgen::net_stream::open_netif: for linux is not implemented yet\n");
    exit(-1);

#else
    throw pgen::exception("pgen::net_stream::open_netif: unknown error");
#endif
}



ssize_t net_stream::write(const void* buffer, size_t bufferlen) {
    ssize_t write_len = ::write(_fd, buffer, bufferlen);
    if (write_len < 0) {
        throw pgen::exception("pgen::net_stream::write::write: ");
    }
    return write_len;
}

ssize_t net_stream::read(void* buffer, size_t bufferlen) {
    ssize_t read_len = ::read(_fd, buffer, bufferlen);
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


size_t net_stream::send(const void* buffer, size_t bufferlen) {
    this->write(buffer, bufferlen);
    return bufferlen;
}


struct bpf_header {
    uint64_t    timestamp;
	uint32_t	caplen;
	uint32_t	datalen;
	uint16_t	hdrlen;	
};

/*
 * TODO
 * This function uses temporary stored memory, so this function
 * has potential that getting speed up.
 */
size_t net_stream::recv(void* buffer, size_t bufferlen) {
    if (_buffer_point == nullptr) {
        _buffer_size_readed = read(_buffer.data(), _buffer.size());
        _buffer_point = _buffer.data();
    }

    struct bpf_header* bh = reinterpret_cast<bpf_header*>(_buffer_point);

    size_t copylen = bh->caplen;
    if (bufferlen < bh->caplen)
        copylen = bufferlen;
    memcpy(buffer, _buffer_point+(bh->hdrlen), copylen);
    _buffer_point += bh->hdrlen;
    _buffer_point += bh->caplen;

    if (_buffer_point - _buffer.data() >= _buffer_size_readed)
        _buffer_point = NULL;

    return copylen;

    // ERASE
    // uint8_t b[4096];
    // this->read(b, sizeof(b));
    // struct pgen::bpf_header* bpfh = (struct pgen::bpf_header*)b;
    // if (bpfh->caplen > bufferlen) {
    //     throw pgen::exception("pgen::net_stream::recv: bufferlen is too small");
    // }
    // memcpy(buf, b+bpfh->hdrlen, bpfh->caplen);
    // return bpfh->caplen;
}



} /* namespace pgen */
