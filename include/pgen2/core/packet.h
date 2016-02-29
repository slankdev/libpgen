

#pragma once


#include <assert.h>
#include <pgen2/util.h>
#include <pgen2/data_container.h>

#include <vector>
#include <pgen2/core/header.h>


namespace pgen {


enum class packet_type {
    unknown,
    ethernet, 
    arp,
    ip,
    icmp,
    tcp,
    udp
};




/**
 * This class provides packet functionality.
 * This is base class, so developers must implement new class
 * inherited this if they want to add new protocol packet class.
 *
 * <h3>Member Variables</h3>
 * The class specilized in one protocol, has header classes 
 * of the following protocol.
 * For example, ICMP packet class has ICMP header class, IP header 
 * class and Ethernet header class.
 **/
class packet {
    protected:
        pgen::packet_type _type;
        pgen::types::data_container  _raw;
        size_t _header_len; // TODO maybe, this variable don't need.
        static const size_t max_header_len = 128; /* [byte] */
        
        std::vector<pgen::header*> headers;

    public:

        packet();
        packet(const packet& rhs);

        /**
         * return packet type 
         **/
        packet_type type() const;
        
        /**
         * set packet content
         **/
        void set_contents(const void* buffer, size_t buflen); 
        
        /**
         * print hexdump      
         **/
        void hex() const;

        /** 
         * return packet raw data
         **/
        const uint8_t* raw() const;

        /**
         * return packet content data 
         **/
        const uint8_t* contents() const;

        /**
         * return packet length
         **/
        size_t length() const; 

        /**
         * Build binary to _rar.
         * call Protocols-Header-class::write() inside this function.
         **/
        void compile(); 
        
        /**
         * Analyze binary as packet.
         * call Protocols-Header-class::read() inside this function.
         **/
        void analyze(const void* buffer, size_t bufferlen);

    public:

        /**
         * return total header length 
         **/
        virtual size_t header_length() const = 0; 

        /**
         * Re init values
         **/
        virtual void clear() = 0;


        // #<{(|*
        //  * Print summary. Seting argument true, print more information.
        //  * @param moreinfo need more information
        //  *|)}>#
        // virtual void summary(bool moreinfo=false) const = 0;
        // virtual void compile() = 0;  
        // // TODO may become normal member func. 
        // virtual void analyze(const void* buffer, size_t buffer_len) = 0; 
        // // TODO it too.

};





} /* namespace pgen */


