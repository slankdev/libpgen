


#include <pgen2/io/stream.h>
#include <pgen2/exception.h>

namespace pgen {
 

file_stream::file_stream() : _fd(NULL) {}
file_stream::~file_stream() {
    this->fclose();
}

void file_stream::fopen(const char* name, const char* mode) {
    _fd = ::fopen(name, mode);
    if (_fd == NULL) {
        std::string err = "pgen::file_stream::fopen:";
        err += name;
        err += " ";
        throw pgen::exception(err.c_str());
    }
}

void file_stream::fclose() {
    if (_fd != NULL) 
        ::fclose(_fd);
}

ssize_t file_stream::write(const void* buf, size_t buflen) {
    size_t number_of_write = ::fwrite(buf, buflen, 1, _fd);   
    if (number_of_write != 1) {
        throw pgen::exception("pgen::file_stream::fwirte::fwite: ");
    }
    return buflen;
}


ssize_t file_stream::read(void* buf, size_t buflen) {
    size_t number_of_read = ::fread(buf, buflen, 1, _fd);   
    if (number_of_read != 1) {
        throw pgen::exception("pgen::file_stream::fread::fread: ");
    }
    return buflen;
}



bool file_stream::feof() const {
    uint8_t c;
    int res = ::fread(&c, 1, 1, _fd);
    if (res != 1 && ::feof(_fd)) {
        return true;
    } else {
        fseek(_fd, -1, SEEK_CUR);
        return false;
    }
}

void file_stream::fflush() const {
    ::fflush(_fd);
}



};
