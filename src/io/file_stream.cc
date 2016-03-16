


#include <pgen2/io/stream.h>
#include <pgen2/exception.h>

namespace pgen {
 

file_stream::file_stream() : _fd(nullptr) {}
file_stream::~file_stream() noexcept {
    this->fclose();
}

void file_stream::fopen(const char* name, const char* mode) {
    _fd = ::fopen(name, mode);
    if (_fd == nullptr) {
        std::string err = "pgen::file_stream::fopen:";
        err += name;
        err += " ";
        throw pgen::exception(err.c_str());
    }
}

void file_stream::fclose() noexcept {
    if (_fd != nullptr) { 
        ::fclose(_fd);
        _fd = nullptr;
    }
}

void file_stream::write(const void* buf, size_t buflen) {
    size_t number_of_write = ::fwrite(buf, buflen, 1, _fd);   
    if (number_of_write != 1) {
        throw pgen::exception("pgen::file_stream::fwirte::fwite: ");
    }
}


size_t file_stream::read(void* buf, size_t buflen) {
    size_t number_of_read = ::fread(buf, buflen, 1, _fd);   
    if (number_of_read != 1) {
        throw pgen::exception("pgen::file_stream::fread::fread: ");
    }
    return buflen;
}



// TODO is this function noexcept?
bool file_stream::feof() const noexcept {
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
    int res = ::fflush(_fd);
    if (res != 0) {
        pgen::exception("pgen::file_stream::fflush: ");
    }
}




bool file_stream::eof() const noexcept { // TODO is this function noexcept?
    /*
     * FIXME
     * If below's code uncomment out, this function always
     * throw exception. Please help...
     */
    return this->feof();
}


void file_stream::flush() const {
    this->fflush();
}


};
