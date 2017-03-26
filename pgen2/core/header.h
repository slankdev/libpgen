

/**
 * @file haeder.h
 * Definition about base header class. We hope to you extending 
 * new packet class with this class.
 **/

#pragma once


#include <stddef.h>


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
         * Re init header elements. 
         **/
        virtual void clear() = 0;

        /**
         * Print summary. Seting argument true, print more information.
         * @param moreinfo need more information
         **/
        virtual void summary(bool moreinfo=false) const = 0;
        
        /**
         * Write header to memory.
         * @param[out] buffer  buffer pointer.
         * @param[in]  buffer length.
         **/
        virtual void write(void* buffer, size_t bufferlen) const = 0;

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

