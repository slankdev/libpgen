
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
        void read(void* buf, size_t buflen);

    public:
        stream();
        ~stream();
        pgen::open_mode mode() const;

        void open(const char* name, pgen::open_mode _mode);
        void close();

        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

};





} /* namespace pgen */
