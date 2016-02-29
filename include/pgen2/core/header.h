
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <pgen2/core/macaddress.h>
#include <pgen2/core/ipaddress.h>



namespace pgen {



/**
 * This class provides protocol header functionality. 
 * This has many elements of protocol header.
 * For example, ethernet_header has ether_dst, ether_src
 * and ether_type as dst, src and type.
 * Developers must implement new protocol header class 
 * inherited this class, if they implement new protocol 
 * packet class. 
 *
 * 
 * <h3>In the class specialized in protocol.</h3>
 * There are elements of protocol header, fx) ether_src...
 * All of elements are public member variables.
 * Developers must be careful packet strangeness in
 * another functions.
 **/
class header {
    public:
        
        /**
         * Write header to memory.
         * @param[out] buffer  buffer pointer.
         * @param[in]  buffer length.
         **/
        virtual void write(void* buffer, size_t bufferlen) = 0;

        /**
         * Read binary as protocol header. 
         * @param[in] buffer  buffer pointer.
         * @param[in] buffer  length.
         **/
        virtual void read(const void* buffer, size_t bufferlen) = 0;

        /**
         * Return header length
         * @return header length.
         **/
        virtual size_t length() const = 0;

};


} /* namespace pgen */

