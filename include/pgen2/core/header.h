
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <pgen2/core/address.h>



namespace pgen {




class ethernet_header {
    public:
        static const size_t max_length = pgen::macaddress::length*2+sizeof(uint16_t);
        static const size_t min_length = pgen::macaddress::length*2+sizeof(uint16_t);

    private:
        macaddress _dst;
        macaddress _src;
        uint16_t   _type;

        uint8_t _raw[max_length];
    public:

        ethernet_header();
        const macaddress& src() const;
        const macaddress& dst() const;
        const uint16_t&   type() const;
        macaddress& src();
        macaddress& dst();
        uint16_t&   type();

        // size_t write(void* buffer, size_t buffer_len);
        void write();
        size_t read(const void* buffer, size_t buffer_len);
        const uint8_t* raw() const; 
        size_t length() const ;
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
// 		size_t write(void* buffer, size_t buffer_len);
// 		size_t read(const void* buffer, size_t buffer_len);
// };
//



} /* pgen */

