

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

#include <pgen2/io/stream.h>


namespace pgen {


/**
 * @enum packet_type
 * This enum is packet_type about protocol type.
 **/
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
        static const size_t max_header_len = 128; /* [byte] */
 
        using header_vect = std::vector<pgen::header*>;
        header_vect headers;

    public:
        
        friend pgen::base_stream& operator << (pgen::base_stream& s, const pgen::packet& pack);

        packet();
        packet(const packet& rhs);

        /**
         * Return packet type as pgen::packet_type.
         * About pgen::packet_type, refer pgen::packet_type
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
         * return total header length.
         * A value returned this function doesn't depend 
         * on the compile();
         **/
        size_t header_length() const; 

        /**
         * Build binary to private mamever variable, _rar.
         * Call header-class::write() inside this function.
         * This function update only raw data.
         **/
        void compile(); 

        /**
         * Analyze binary as packet.
         * Call header-class::read() inside this function.
         * This function update only protocol member variables.
         * This function doesn't update raw data.
         **/
        void analyze(const void* buffer, size_t bufferlen);

    public:

        /**
         * Re init values.
         * Developer must override this.
         **/
        virtual void clear() = 0;

    protected:

        /**
         * Init headers array using compile() and analyze().
         * Developer must override this.
         **/
        virtual void init_headers() = 0;

};





} /* namespace pgen */


