
#pragma once

#include <vector>
#include <stdio.h>
#include <stdint.h>

#include <pgen2/io/util.h>
#include <pgen2/types/data_container.h>


namespace pgen { 
namespace types { 

class data_container {
    private:
        std::vector<uint8_t> _vec;
        size_t _pivot;
    public:
        // data_container(size_t pivot=128):_pivot(pivot){}
        data_container(size_t pivot=128);
        size_t size() const;
        size_t pivot() const;
        const uint8_t* get_content() const;

        void write_before(int index, uint8_t* buf, size_t buflen);
        void set_content(const uint8_t* buf, size_t buflen);
        void delete_content();

        void hex() const ;
};

} /* namespace types */
} /* namespace pgen  */
