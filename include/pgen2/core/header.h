
#pragma once


#include <stdio.h>
#include <pgen2.h>



namespace pgen {
namespace core {
namespace header {


class header {
    private:
    public:
        virtual size_t write_header(void* buffer, size_t buffer_len) = 0;
        virtual size_t read_header(const void* buffer, size_t buffer_len) = 0;
};


class ethernet : public header {
    private:
        macaddress _dst;
        macaddress _src;
        uint16_t   _type;

    public:
        ethernet();
        const macaddress& src() const;
        const macaddress& dst() const;
        const uint16_t&   type() const;
        macaddress& src();
        macaddress& dst();
        uint16_t&   type();

        size_t write_header(void* buffer, size_t buffer_len);
        size_t read_header(const void* buffer, size_t buffer_len);

};




} /* header */
} /* core */
} /* pgen */

