
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <pgen2/core/macaddress.h>
#include <pgen2/core/ipaddress.h>



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
        // void write();
        // const uint8_t* raw() const; 

        ethernet_header();

        const macaddress& src() const;
        const macaddress& dst() const;
        uint16_t   type() const;

        void src(const macaddress& a);
        void dst(const macaddress& a);
        void type(uint16_t t);

        void write(void* buffer, size_t buffer_len);
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};



class ipv4_header {
    public:
        static const size_t min_length
            = 12+pgen::ipv4address::_length*2;
        static const size_t max_length
            = min_length+40;

	private:

        uint8_t    _hlen; // this is special field, and 4bit field
        uint8_t    _tos;
        uint16_t   _tot_len; // this is special field 
        uint16_t   _id;
        uint16_t   _frag_off;
        uint8_t    _ttl;
        uint8_t    _protocol;
        uint16_t   _check;
        ipv4address  _src;
        ipv4address  _dst;     
        
        uint8_t _option[40];

        // uint8_t _raw[max_length4];
    public:
        // const uint8_t* raw() const; 

        ipv4_header();
    
        uint8_t  hlen() const ;    
        uint8_t  tos() const ;
        uint16_t tot_len() const ;
        uint16_t id() const;
        uint16_t frag_off() const ;
        uint8_t  ttl() const ;
        uint8_t  protocol() const ;
        uint16_t check() const ;
        const ipv4address& src() const ;
        const ipv4address& dst() const ;     
        const uint8_t* option() const ;

        void hlen(uint8_t n);    
        void tos(uint8_t n);
        void tot_len(uint16_t n);
        void id(uint16_t n);
        void frag_off(uint16_t n);
        void ttl(uint8_t n );
        void protocol(uint8_t n);
        void check(uint16_t n);
        void src(const ipv4address& n);
        void dst(const ipv4address& n);
        void option(const void* buf, size_t buflen);

        void write(void* buffer, size_t buffer_len);
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};


} /* pgen */

