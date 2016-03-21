
#pragma once


#include <pgen2/io/stream.h>
#include <pgen2/arch/arch.h>

#include <sys/socket.h> // for struct sockaddr
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>




namespace pgen {

class net_stream : public base_stream {
    private:
        int _fd;
#if defined(__PGEN_OSX)
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


} /* namespace pgen */
