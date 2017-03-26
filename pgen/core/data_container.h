
#pragma once

#include <vector>
#include <stddef.h>
#include <stdint.h>



namespace pgen {
namespace types {


class data_container {
    private:
        std::vector<uint8_t> _vec;
        size_t _pivot;
    public:
        data_container(size_t pivot=128);

        uint8_t* data() const noexcept;
        size_t size() const noexcept;
        size_t pivot() const noexcept;

        void write_before(size_t index, const void* buf, size_t buflen);
        void set_content(const void* buf, size_t buflen);
        void delete_content();

        uint8_t& operator [] (int n) noexcept;
};


} /* namespace types */
} /* namespace pgen  */

/*
 * Function Implementation
 */


#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <pgen/exception.h>


namespace pgen {
namespace types {



inline data_container::data_container(size_t pivot) : _pivot(pivot)
{
    _vec.resize(_pivot, 0);
}

inline uint8_t* data_container::data() const noexcept
{
    return const_cast<uint8_t*>(_vec.data());
}

inline size_t data_container::size() const noexcept
{
    return _vec.size();
}

inline size_t data_container::pivot() const noexcept
{
    return _pivot;
}

inline void data_container::write_before(size_t index,
                    const void* buf, size_t buflen)
{
    if (index < buflen)
        throw pgen::exception(
           "pgen::types::data_container::write_before: buflen is too large");

    // TODO is it OK?
    // std::copy_n(buf, buflen, &_vec[index - buflen]);
    //
    const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
    std::copy_n(p, buflen, &_vec[index - buflen]);

    // ERASE
    // std::vector<uint8_t>::iterator it = _vec.begin();
    // it += index - buflen;
    // const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
    // for (size_t i=0; i<buflen; ++i, ++it)
    //     *it = p[i];
}

inline void data_container::set_content(const void* buf, size_t buflen)
{
    delete_content();
    _vec.insert(_vec.end(), reinterpret_cast<const uint8_t*>(buf),
            reinterpret_cast<const uint8_t*>(buf) + buflen);
}

inline void data_container::delete_content()
{
    _vec.resize(pivot());
}


inline uint8_t& data_container::operator [] (int n) noexcept
{
    return _vec[n];
}



} /* namespace types */
} /* namespace pgen */
