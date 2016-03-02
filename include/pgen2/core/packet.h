

/**
 * @file packet.h
 * Definition about base packet class. We hope to you extending 
 * new packet class with this class.
 **/


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
         * Return packet type as pgen::packet_type.
         * About pgen::packet_type, refer <>
         **/
        packet_type type() const;
        
        /**
         * Set packet contents.
         **/
        void set_contents(const void* buffer, size_t buflen); 
        
        /**
         * Print hexdump      
         **/
        void hex() const;

        /** 
         * Return packet raw data pointer. 
         * User can't read and write this area.
         * A value returned this function depends on the 
         * compile();
         **/
        const uint8_t* raw() const;

        /**
         * Return packet content data.
         * User can't read and write this area.
         **/
        const uint8_t* contents() const;

        /**
         * Return packet length.
         * A value returned this function doesn't depend 
         * on the compile();
         **/
        size_t length() const; 

        /**
         * Build binary to private mamever variable, _rar.
         * Call header-class::write() inside this function.
         * This function update only raw data and packet length.
         **/
        void compile(); 
        
        /**
         * Analyze binary as packet.
         * Call header-class::read() inside this function.
         * This function update only protocol member variables.
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

    private:

        /**
         * Init headers array using compile() and analyze().
         **/
        virtual void init_headers() = 0;


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


