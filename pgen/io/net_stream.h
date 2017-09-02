
#pragma once


#include <pgen/io/stream.h>
#include <pgen/arch.h>

#include <sys/socket.h> // for struct sockaddr
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>





/* following include is only for Impl */
#include <pgen/io/net_stream.h>
#include <pgen/exception.h>
#include <pgen/arch.h>

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

#if defined(PGEN_OSX)
#include <net/if_dl.h>
#include <net/bpf.h>
#elif defined(PGEN_LINUX)
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <pgen/arch.h>

namespace pgen {

class net_stream : public base_stream {
    private:
        int _fd;
#if defined(PGEN_OSX)
        std::vector<uint8_t> _buffer;
        uint8_t* _buffer_point;
        ssize_t _buffer_size_readed;
#endif

        void open_netif(const char* name, size_t buffer_size=0);

        void ioctl(unsigned long l, void* p);
        void bind(const struct sockaddr* sa, size_t len);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);
    public:

        net_stream();
        net_stream(const char* name, pgen::open_mode mode);
        ~net_stream() noexcept;

        void open(const char* name, pgen::open_mode);
        void close() noexcept;
        void send(const void* buffer, size_t bufferlen);
        size_t recv(void* buffer, size_t bufferlen);
};


/*
 * Function Implementation
 */



inline net_stream::net_stream() : _fd(-1) {}


inline net_stream::net_stream(const char* name, pgen::open_mode mode) : net_stream() {
    open(name, mode);
}


inline net_stream::~net_stream() noexcept {
    close();
}


inline void net_stream::open_netif(const char* name, size_t buffer_size) {
#if defined(PGEN_OSX)

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
	ioctl(BIOCPROMISC, nullptr);     // set promisc
	ioctl(BIOCIMMEDIATE, &one);   //if recv packet then call read fast
	ioctl(BIOCSSEESENT, &one);    // set recv sendPacket
	ioctl(BIOCFLUSH, nullptr);       // flush recv buffer
	ioctl(BIOCSHDRCMPLT, &one);   // no complite src macaddr
    _buffer_point = nullptr;           // init _buffer_point

#elif defined(PGEN_LINUX)

    struct ifreq ifreq;
    struct sockaddr_ll sa;

    _fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (_fd < 0) {
        throw pgen::exception("pgen::net_stream::open_netif: ");
    }

    // TODO fix strncpy to strcpy
    memset(&ifreq, 0, sizeof(ifreq));
    strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name)-1);
    ioctl(SIOCGIFINDEX, &ifreq);

    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ALL);
    sa.sll_ifindex  = ifreq.ifr_ifindex;
    // TODO impl bind rapped net_stream;
    bind(reinterpret_cast<sockaddr*>(&sa), sizeof(sa));

    ioctl(SIOCGIFFLAGS, &ifreq);
    ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
    ioctl(SIOCSIFFLAGS, &ifreq);

#else
#error "unknown arch"
#endif
}



inline void net_stream::write(const void* buffer, size_t bufferlen) {
    ssize_t write_len = ::write(_fd, buffer, bufferlen);
    if (static_cast<size_t>(write_len) != bufferlen) {
        throw pgen::exception(
                "pgen::net_stream::write::write: write comletion faild");
    } else if (write_len < 0) {
        throw pgen::exception("pgen::net_stream::write::write: ");
    }
}

inline size_t net_stream::read(void* buffer, size_t bufferlen) {
    ssize_t read_len = ::read(_fd, buffer, bufferlen);

    if (read_len < 0) {
        throw pgen::exception("pgen::net_stream::read::read: ");
    } else if (read_len == EINTR) {
        return read(buffer, bufferlen);
    }

    return static_cast<size_t>(read_len);
}


inline void net_stream::bind(const struct sockaddr* sa, size_t len) {
    int res = ::bind(_fd, sa, len);
    if (res < 0) {
        throw pgen::exception("pgen::net_stream::bind: ");
    }
}



inline void net_stream::ioctl(unsigned long l, void* p) {
    int res = ::ioctl(_fd, l, p);
    if (res < 0) {
        throw pgen::exception("pgen::net_stream::ioctl: ");
    }
}


inline void net_stream::open(const char* name, pgen::open_mode mode) {
    if (mode == pgen::open_mode::netif) {
        this->open_netif(name);
    } else {
        throw pgen::exception("pgen::net_stream::open: unknown mode");
    }
}


inline void net_stream::close() noexcept {
    if (_fd >= 0) {
        ::close(_fd);
        _fd = -1;
    }
}


inline void net_stream::send(const void* buffer, size_t bufferlen) {
    this->write(buffer, bufferlen);
}


struct bpf_header {
    uint64_t    timestamp;
	uint32_t	caplen;
	uint32_t	datalen;
	uint16_t	hdrlen;
};


inline size_t net_stream::recv(void* buffer, size_t bufferlen) {
#if defined(PGEN_OSX)
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
        _buffer_point = nullptr;

    return copylen;
#elif defined(PGEN_LINUX)
    ssize_t recvlen = read(buffer, bufferlen);
    if (recvlen < 0) {
        throw pgen::exception("pgen::net_stream::recv: ");
    }
    return recvlen;
#else
#error "unknown arch"
#endif

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
