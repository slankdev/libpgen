
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pgen2/types.h>


namespace pgen {


enum class open_mode {
    pcap_read = 0,
    pcap_write,
    pcapng_read,
    pcapng_write,
    netif
};


class base_stream {
    protected:
        pgen::open_mode _mode;

    public:
        pgen::open_mode mode() const;
        void print_mode() const;

        virtual void open(const char* name, pgen::open_mode mode) = 0;
        virtual void close() = 0;
        virtual size_t send(const void* buffer, size_t bufferlen) = 0;
        virtual size_t recv(void* buffer, size_t bufferlen) = 0;
};


class file_stream : public base_stream {
    protected:
        FILE* _fd;

        void fopen(const char* name, const char* mode);
        void fclose();
        void fwrite(const void* buf, size_t buflen);
        size_t fread(void* buf, size_t buflen);

        bool feof() const;  
        void fflush() const;

    public:
        file_stream();
        ~file_stream();
};


class pcap_stream : public file_stream {
    private:
        struct pcap_file_header file_header;
    public:

        void open(const char* name, pgen::open_mode _mode);
        void close();
        size_t send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

        bool eof() const;   /* pcap_read only  */
        void flush() const; /* pcap_write only */
};


class net_stream : public base_stream {
    private:
        int _fd;
        pgen::open_mode _mode;

        void open_netif(const char* name);
        void ioctl_c(unsigned long l, const void* p);
        void write(const void* buf, size_t buflen);
        size_t read(void* buf, size_t buflen);
    public:

        net_stream();
        ~net_stream();

        void open(const char* name, pgen::open_mode);
        void close();
        size_t send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);
};


// class pcapng_stream : public file_stream {
//     public:
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
