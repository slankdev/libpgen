

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>

#include <pgen/io/stream.h>
#include <pgen/exception.h>




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




/*
 * Function Implementation
 */



inline file_stream::file_stream() : _fd(nullptr) {}
inline file_stream::~file_stream() noexcept {
    this->fclose();
}

inline void file_stream::fopen(const char* name, const char* mode) {
    _fd = ::fopen(name, mode);
    if (_fd == nullptr) {
        std::string err = "pgen::file_stream::fopen:";
        err += name;
        err += " ";
        throw pgen::exception(err.c_str());
    }
}

inline void file_stream::fclose() noexcept {
    if (_fd != nullptr) {
        ::fclose(_fd);
        _fd = nullptr;
    }
}

inline void file_stream::write(const void* buf, size_t buflen) {
    size_t number_of_write = ::fwrite(buf, buflen, 1, _fd);
    if (number_of_write != 1) {
        throw pgen::exception("pgen::file_stream::fwirte::fwite: ");
    }
}


inline size_t file_stream::read(void* buf, size_t buflen) {
    size_t number_of_read = ::fread(buf, buflen, 1, _fd);
    if (number_of_read != 1) {
        throw pgen::exception("pgen::file_stream::fread::fread: ");
    }
    return buflen;
}



// TODO is this function noexcept?
inline bool file_stream::feof() const noexcept {
    uint8_t c;
    int res = ::fread(&c, 1, 1, _fd);
    if (res != 1 && ::feof(_fd)) {
        return true;
    } else {
        // fseek(_fd, -1, SEEK_CUR); // ERASE
        ungetc(c, _fd);
        return false;
    }
}

inline void file_stream::fflush() const {
    int res = ::fflush(_fd);
    if (res != 0) {
        pgen::exception("pgen::file_stream::fflush: ");
    }
}



inline void file_stream::close() noexcept {
    this->fclose();
}



inline bool file_stream::eof() const noexcept { // TODO is this function noexcept?
    return this->feof();
}


inline void file_stream::flush() const {
    this->fflush();
}






} /* namespace pgen */
