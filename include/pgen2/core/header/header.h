
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <pgen2/core/address.h>



namespace pgen {




class ethernet_header {
    public:
        static const size_t max_length 
            = pgen::macaddress::length*2+sizeof(uint16_t);
        static const size_t min_length 
            = pgen::macaddress::length*2+sizeof(uint16_t);

    private:
        macaddress _dst;
        macaddress _src;
        uint16_t   _type;

        // uint8_t _raw[max_length];
    public:

        ethernet_header();

        const macaddress& src() const;
        const macaddress& dst() const;
        uint16_t   type() const;

        void src(const macaddress& a);
        void dst(const macaddress& a);
        void type(uint16_t t);

        // void write();
        void write(void* buffer, size_t buffer_len);
        size_t read(const void* buffer, size_t buffer_len);
        // const uint8_t* raw() const; 
        size_t length() const ;
};



class ip_header {
    public:
        static const size_t min_length4 
            = 12+pgen::ipaddress::length4*2;
        static const size_t max_length4 
            = min_length4+40;

	private:
        // uint8_t _raw[max_length4];

        uint8_t    _hlen; // this is special field, and 4bit field
        uint8_t    _tos;
        uint16_t   _tot_len; // this is special field 
        uint16_t   _id;
        uint16_t   _frag_off;
        uint8_t    _ttl;
        uint8_t    _protocol;
        uint16_t   _check;
        ipaddress  _src;
        ipaddress  _dst;     

    public:

        ip_header();
    
        uint8_t  hlen() const ;    
        uint8_t  tos() const ;
        uint16_t tot_len() const ;
        uint16_t id() const;
        uint16_t frag_off() const ;
        uint8_t  ttl() const ;
        uint8_t  protocol() const ;
        uint16_t check() const ;
        const ipaddress& src() const ;
        const ipaddress& dst() const ;     

        void hlen(uint8_t n);    
        void tos(uint8_t n);
        void tot_len(uint16_t n);
        void id(uint16_t n);
        void frag_off(uint16_t n);
        void ttl(uint8_t n );
        void protocol(uint8_t n);
        void check(uint16_t n);
        void src(const ipaddress& n);
        void dst(const ipaddress& n);

        void write(void* buffer, size_t buffer_len);
        size_t read(const void* buffer, size_t buffer_len);
        // const uint8_t* raw() const; 
        size_t length() const ;
};


} /* pgen */

