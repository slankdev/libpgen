
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
