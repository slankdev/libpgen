
#pragma once


#include <stddef.h>
#include <vector>



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



class file_stream : public base_stream {
    protected:
        FILE* _fd;

        void fopen(const char* name, const char* mode);
        void fclose() noexcept;
        void write(const void* buf, size_t buflen);
        size_t read(void* buf, size_t buflen);

        bool feof() const noexcept;  
        void fflush() const;

    public:
        file_stream();
        ~file_stream() noexcept;

        bool eof() const noexcept;   /* read only  */
        void flush() const; /* write only */
};



struct pcap_file_header {
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t timezone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};


class pcap_stream : public file_stream {
    private:
        struct pcap_file_header file_header;
    public:

        pcap_stream();
        pcap_stream(const char* name, pgen::open_mode mode);

        void open(const char* name, pgen::open_mode _mode);
        void close() noexcept;
        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

};



class net_stream : public base_stream {
    private:
        int _fd;
        std::vector<uint8_t> _buffer;
        uint8_t* _buffer_point;
        ssize_t _buffer_size_readed;

        void open_netif(const char* name, size_t buffer_size=0);

        void ioctl(unsigned long l, void* p);
        void write(const void* buffer, size_t bufferlen);
        size_t read(void* buffer, size_t bufferlen);
    public:

        net_stream();
        net_stream(const char* name, pgen::open_mode mode);
        ~net_stream() noexcept;

        void open(const char* name, pgen::open_mode);
        void close() noexcept;
        void send(const void* buffer, size_t bufferlen);
        size_t recv(void* buffer, size_t bufferlen);
};





} /* namespace pgen */
