
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <pgen2/core/macaddress.h>
#include <pgen2/core/ipaddress.h>



namespace pgen {



/* header class
 *
 * Description
 *      This class provides protocol header functionality. 
 *      This has many elements of protocol header.
 *      For example, ethernet_header has ether_dst, ether_src
 *      and ether_type as dst, src and type.
 *      Developers must implement new protocol header class 
 *      inherited this class, if they implement new protocol 
 *      packet class.
 *
 * Member Variables
 *      There are elements of protocol header. 
 *      All of elements are public member variables.
 *      Developers must be careful packet strangeness in
 *      another functions.
 *
 * Member Functions
 *      This class has some pure virtual functions, so
 *      developers must override them, if implementing 
 *      new header class.
 *
 *      void write(void* buffer, size_t bufferlen)
 *      Write header binary to buffer. 
 *      If bufferlen is too small, throwing pgen::exception.
 *      
 *      void read(const void* buffer, size_t bufferlen)
 *      Read binary as header. 
 *      If bufferlen is too small, throwing pgen::exception.
 *      
 *      size_t length()
 *      Return header length.
 */
class header {
    public:
        virtual void write(void* buffer, size_t bufferlen) = 0;
        virtual void read(const void* buffer, size_t bufferlen) = 0;
        virtual size_t length() const = 0;
};




class ethernet_header : header {
    public:
        static const size_t min_length 
            = pgen::macaddress::length*2+sizeof(uint16_t);
        static const size_t max_length 
            = min_length;

        macaddress dst;
        macaddress src;
        uint16_t   type;

        void write(void* buffer, size_t buffer_len);
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};




class ipv4_header : header {
    public:
        static const size_t min_length
            = 12+pgen::ipv4address::length*2;
        static const size_t max_length
            = min_length+40;

        uint8_t      hlen; // this is special field, and 4bit field
        uint8_t      tos;
        uint16_t     tot_len; // this is special field 
        uint16_t     id;
        uint16_t     frag_off;
        uint8_t      ttl;
        uint8_t      protocol;
        uint16_t     check;
        ipv4address  src;
        ipv4address  dst;     
        uint8_t option[max_length-min_length];

        void write(void* buffer, size_t buffer_len);
        void read(const void* buffer, size_t buffer_len);
        size_t length() const ;
};





} /* pgen */

