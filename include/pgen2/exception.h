

#pragma once

#include <exception>
#include <string>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <sstream>


namespace pgen {
 


class exception : public std::exception {
    private:
        std::string str;
    public:
        explicit exception(const char* s="") noexcept {
            int e = errno;
            str = s; 
            if (e != 0)
                str += strerror(e);
        } 

        template<class T>
        exception& operator<<(const T& t) noexcept {
            std::ostringstream os;
            os << " " << t ;
            str += os.str();
            return *this;
        }
        const char* what() const noexcept {
            return str.c_str(); 
        }
};



} /* namespace pgen */
