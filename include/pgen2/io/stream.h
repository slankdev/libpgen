
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pgen2/types.h>
#include <pgen2/exception.h>
#include <pgen2/io/util.h>



namespace pgen {


class stream {
    private:
        FILE* _offline_fd;
        pgen::open_mode _mode;

        void open_file(const char* fname, const char* mode);
        
        void write(const void* buf, size_t buflen);
        size_t read(void* buf, size_t buflen);
    
        stream(const stream&) = delete;
        stream& operator=(const stream&) = delete;

    public:

        stream();
        ~stream();
        pgen::open_mode mode() const;

        void open(const char* name, pgen::open_mode _mode);
        void close();

        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

        bool eof() const;
        void flush() const;

    public:
        FILE* fd() const {return _offline_fd;} // Todo: Remove after implementing.
};





} /* namespace pgen */
