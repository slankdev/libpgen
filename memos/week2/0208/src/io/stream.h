
namespace pgen {
namespace io {

enum mode {
	pcap_read,
	pcap_write,
	pcapng_read,
	pcapng_write,
	netif
};

class stream {
	private:
	public:
		stream(void);
		stream(const char* ifname, mode m);
		~stream(void);

		void open(const char* ifname, mode m);
		void close(void);
};

} /* namespace io */
} /* namespace pgen */
