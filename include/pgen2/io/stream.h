
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <stdio.h>




namespace pgen {


enum class open_mode {
    pcap_read = 0,
    pcap_write,
    pcapng_read,
    pcapng_write,
    netif
};



/**
 * This class is base class of pgen stream classes.
 * Developers can implement new stream class inherited this.
 **/
class base_stream {
    protected:
        pgen::open_mode _mode;

    public:
        pgen::open_mode mode() const noexcept; /**< return stream mode */
        void print_mode() const noexcept;      /**< print stream mode  */
        
        /**
         * Open stream 
         **/
        virtual void open(const char* name, pgen::open_mode mode) = 0;
        
        /**
         * Close stream 
         **/
        virtual void close() noexcept = 0;

        /**
         * Send data from stream
         * @param [in] buffer send buffer.
         * @param [in] bufferlen send buffer length.
         **/
        virtual void send(const void* buffer, size_t bufferlen) = 0;

        /**
         * Recieve data from stream
         * @param [out] buffer send buffer.
         * @param [in] bufferlen send buffer length.
         **/
        virtual size_t recv(void* buffer, size_t bufferlen) = 0;

};






} /* namespace pgen */
