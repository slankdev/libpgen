
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pgen2/types.h>
#include <pgen2/exception.h>
#include <pgen2/io/util.h>



namespace pgen {



class stream {
    private:
        int   _net_fd;
        FILE* _file_fd;
        pgen::open_mode mode;

        stream(const stream&) = delete;
        stream& operator=(const stream&) = delete;

        FILE* open_pcap_read(const char* interface) {
            return pgen::util::open_pcap(interface, pgen::open_mode::pcap_read);
        }
        FILE* open_pcap_write(const char* interface) {
            return pgen::util::open_pcap(interface, pgen::open_mode::pcap_write);
        }
        int   open_netif(const char* interface) {
            printf("open %s [netif] \n" , interface);
            return -1;
        }

    public:
        stream() : _net_fd(-1), _file_fd(NULL) {}
        ~stream() {
            this->close();
        }

        void open(const char* interface, pgen::open_mode m) {
            this->mode = m;

            switch (mode) {
                case open_mode::pcap_read:
                    _file_fd = open_pcap_read(interface);
                    break;

                case open_mode::pcap_write:
                    _file_fd = open_pcap_write(interface);
                    break;

                case open_mode::pcapng_read:
                    throw pgen::exception("pcapng operation is not support yet");
                    break;

                case open_mode::pcapng_write:
                    throw pgen::exception("pcapng operation is not support yet");
                    break;

                case open_mode::netif:
                    throw pgen::exception("netif operation is not support yet");
                    _net_fd = open_netif(interface);
                    break;
                default:
                    break;
            }
        }

        void close() {
            if (_net_fd >= 0)
                ::close(_net_fd);
            if (_file_fd != NULL)
                fclose(_file_fd);
        }

        size_t send(const void* buffer, size_t bufferlen) {
            if (mode == pgen::open_mode::pcap_write) {
                return pgen::util::send_to_pcap(_file_fd, buffer, bufferlen);
            } 
            throw pgen::exception("pgen::stream::send: not implement");
        }  

        size_t recv(void* buffer, size_t bufferlen) {
            if (buffer == NULL) return 0;
            return bufferlen;
        }

};
   


} /* namespace pgen */
