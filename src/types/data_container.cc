

#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <pgen2/types/data_container.h>


namespace pgen {
namespace types {
        


data_container::data_container(size_t pivot) : _pivot(pivot) { 
    _vec.resize(_pivot, 0);
}

size_t data_container::size() const {
    return _vec.size(); 
}

const uint8_t* data_container::get_content() const {
    return _vec.data() + _pivot; 
}

size_t data_container::pivot() const {
    return _pivot;      
}

void data_container::write_before(int index, uint8_t* buf, size_t buflen) {
    std::vector<uint8_t>::iterator it = _vec.begin();
    it += index - buflen;
    for (int i=0; i<buflen; i++, it++) {
        *it = buf[i];
    }
}

void data_container::set_content(const uint8_t* buf, size_t buflen) {
    delete_content();
    _vec.insert(_vec.end(), buf, buf+buflen);
}

void data_container::delete_content() {
    std::vector<uint8_t>::iterator it = _vec.begin();
    it += _pivot;
    _vec.erase(it, _vec.end());       
}

void data_container::hex() const {
    pgen::io::util::hex((void*)_vec.data(), _vec.size());
}

} /* namespace types */
} /* namespace pgen */
