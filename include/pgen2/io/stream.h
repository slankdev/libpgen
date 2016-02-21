
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pgen2/types.h>
#include <pgen2/exception.h>
#include <pgen2/io/util.h>



namespace pgen {



enum class open_mode {
    pcap_read,
    pcap_write,
    pcapng_read,
    pcapng_write,
    netif
};




class base_stream {
    public:
        void print();

        // virtual ~base_stream(){}

        virtual void open(const char* name, pgen::open_mode mode) = 0;
        virtual void close() = 0;
        virtual size_t send(const void* buffer, size_t bufferlen) = 0;
        virtual size_t recv(void* buffer, size_t bufferlen) = 0;
        virtual pgen::open_mode mode() const = 0;
};


class pcap_stream : public base_stream {
    private:
        FILE* _fd;
        pgen::open_mode _mode;
        
        void fopen(const char* name, const char* mode);
        void fwrite(const void* buf, size_t buflen);
        size_t fread(void* buf, size_t buflen);
    
    public:

        pcap_stream();
        ~pcap_stream();
        pgen::open_mode mode() const;

        void open(const char* name, pgen::open_mode _mode);
        void close();
        size_t send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

        bool eof() const;
        void flush() const;
        FILE* fd() const {return _fd;} // Todo: Remove after implementing.
};


class net_stream : public base_stream {
    private:
        int _fd;
        pgen::open_mode _mode;

        void open_netif(const char* name);
        void ioctl(unsigned long l, void* p);
        void ioctl_c(unsigned long l, const void* p);
        void write(const void* buf, size_t buflen);
        size_t read(void* buf, size_t buflen);

    public:

        net_stream();
        ~net_stream();
        pgen::open_mode mode() const;

        void open(const char* name, pgen::open_mode);
        void close();
        size_t send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);
};


// class pcapng_stream : public base_stream {
//     private:
//         FILE* _fd;
//         pgen::open_mode _mode;
//
//         void fopen(const char* name, const char* mode);
//         void fwrite(const void* buf, size_t buflen);
//         size_t fread(void* buf, size_t buflen);
//     
//     public:
//
//         pcapng_stream();
//         ~pcapng_stream();
//         pgen::open_mode mode() const;
//
//         void open(const char* name, pgen::open_mode _mode);
//         void close();
//         size_t send(const void* buf, size_t buflen);
//         size_t recv(void* buf, size_t buflen);
//
//         bool eof() const;
//         void flush() const;
// };



} /* namespace pgen */
