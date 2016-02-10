
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


// class ip : public header {
// 	private:
// 		uint8_t    _hlen:4;
// 		uint8_t    _tos;
// 		uint16_t   _tot_len;
// 		uint16_t   _id
// 		uint16_t   _frag_off;
// 		uint8_t    _ttl;
// 		uint8_t    _protocol;
// 		uint16_t   _check;
// 		ipaddr     _src;
// 		ipaddr     _dst;     
//
// 	public:
// 		const uint8_t&    hlen() const ;    
// 		const uint8_t&    tos() const ;
// 		const uint16_t&   tot_len() const ;
// 		const uint16_t&   id() const;
// 		const uint16_t&   frag_off() const ;
// 		const uint8_t&    ttl() const ;
// 		const uint8_t&    protocol() const ;
// 		const uint16_t&   check() const ;
// 		const ipaddr&     src() const ;
// 		const ipaddr&     dst() const ;     
// 	
// 		uint8_t&    hlen();    
//         uint8_t&    tos();
//         uint16_t&   tot_len();
//         uint16_t&   id();
//         uint16_t&   frag_off();
//         uint8_t&    ttl();
//         uint8_t&    protocol();
//         uint16_t&   check();
// 		ipaddr&     src();
// 		ipaddr&     dst();
//
// 		size_t write_header(void* buffer, size_t buffer_len);
// 		size_t read_header(const void* buffer, size_t buffer_len);
// };
//



} /* header */
} /* core */
} /* pgen */

