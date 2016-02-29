

#pragma once


#include <assert.h>
#include <pgen2/util.h>
#include <pgen2/data_container.h>




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



/* packet class
 *
 * Description
 *      This class provides packet functionality.
 *      This is base class, so developers must implement new class
 *      inherited this if they want to add new protocol packet class.
 *
 * Member Variables
 *      The class specilized in one protocol, has header classes 
 *      of the following protocol.
 *      For example, ICMP packet class has ICMP header class, IP header 
 *      class and Ethernet header class.
 *
 * Member Functions
 *      This class has some pure virtual functions, so
 *      developers must override them, if implementing 
 *      new header class.
 *
 *      size_t header_length() const
 *      Return the length of the total headers.
 *
 *      void clear()
 *      Restore all members to the initial values. 
 *
 *      void compile()
 *      Build vinary to member value, _raw from protocol headers. 
 *      This is sendable binary as packet.
 *
 *      void analyze(const void* buffer, size_t buffer_len)
 *      Analyze binary as packet, and refrect to member values.
 *
 *      void summary(bool moreinfo=false) const
 *      Print summary. Seting argument true, print more information.
 */
class packet {
    protected:
        pgen::packet_type _type;
        pgen::types::data_container  _raw;
        size_t _header_len; // TODO maybe, this variable don't need.
        static const size_t max_header_len = 128; /* [byte] */

    public:

        packet_type type() const;
        void set_contents(const void* buffer, size_t buflen);
        void hex() const;

        const uint8_t* raw() const;
        const uint8_t* contents() const;
        size_t length() const;

        virtual size_t header_length() const = 0;
        virtual void clear() = 0;
        virtual void compile() = 0;  // TODO may become normal member func. 
        virtual void analyze(const void* buffer, size_t buffer_len) = 0; // TODO it too.
        virtual void summary(bool moreinfo=false) const = 0;
};









} /* namespace pgen */


