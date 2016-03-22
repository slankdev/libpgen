

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>

#include <pgen2/io/stream.h>




namespace pgen {


class file_stream : public base_stream {
    protected:
        FILE* _fd;

        void fopen(const char* name, const char* mode);
        void fclose() noexcept;
        void write(const void* buf, size_t buflen);
        size_t read(void* buf, size_t buflen);

        bool feof() const noexcept;  
        void fflush() const;

    public:
        file_stream();
        ~file_stream() noexcept;
        void close() noexcept;

        bool eof() const noexcept;   /* read only  */
        void flush() const; /* write only */
};


} /* namespace pgen */
