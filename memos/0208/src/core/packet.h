
#pragma once


#include <vector> 
#include <sstream>
#include <assert.h>


namespace pgen {
namespace core {

enum packet_type {
	unknown,
	ethernet, 
	arp,
	ip,
	icmp,
	tcp,
	udp
}





class exception : std::exception {
	private:
		std::string str;
	public:
		explicit exception(const std::string& s="") : str(s) {} 
		template<class T>
		exception& operator<<(const T& t) {
			std::ostringstream os;
			os << " " << t ;
			str += os.str();
			return *this;
		}
		const char* what() const {
			return str.c_str();	
		}
};




using byte_array = std::vector<uint8_t>;

class packet {
	protected:
		
		packet_type _type;
		byte_array _raw;
		virtual void compile() = 0;
		
		static const size_t max_header_len = 128; /* [byte] */
		
	public:
		void  set_contents(const void* buffer, size_t buflen);
		
		packet() : _raw(max_header_len) {}
		virtual ~packet();
		

		const uint8_t* raw() const {
			size_t headerlen = get_header_length();
			assert(headerlen < max_header_len);
			return contents() - headerlen;	
		}

		size_t length() const {
			assert(_raw.size() >= max_header_len);
			return _raw.size() - max_header_len + get_header_length();
		}

		const uint8_t* contents() const {
			assert(_raw.size() >= max_header_len);
			return raw() + max_header_len;	
		}
		virtual size_t get_header_length() const = 0;


		packet_type get_type() const {
			return _type;	
		}
		void hex() const {
			pgen_hex(raw(), length());
		}

		virtual void clear() = 0;

		virtual void analyze(const void* buffer, size_t buffer_len) = 0;

		virtual void summary(bool moreinfo=false) const = 0;
		virtual void help() const = 0;
};








class ethernet : packet {
	private:
		void compile();
		
	public:
		ethernet_header ethernet;

		ethernet();
		~ethernet();
		void clear();

		void analyze(const void* buffer, size_t buffer_len);
		void summary(bool moreinfo=false) const;
		void help() const;
};


class ethernet : packet {
	private:
		macaddr _dst;
		macaddr _src;
		bit16   _type;

		void compile();

	public:
		const macaddr& src() const;
		const macaddr& dst() const;
		macaddr& src();
		macaddr& dst();
		size_t write_header(void* buffer, size_t buffer_len);
		size_t read_header(const void* buffer, size_t buffer_len);

		ethernet();
		~ethernet();
		void clear();

		void analyze(const void* buffer, size_t buffer_len);
		void summary(bool moreinfo=false) const;
		void help() const;
};




class ip : packet {
	private:
		void compile();

	public:
		ethernet_header ethernet;
		ip_header ip;

		ip(bool isIPv6=false);
		~ip();
		void clear();

		void analyze(const void* buffer, size_t buffer_len);
		void summary(bool moreinfo=false) const;
		void help() const;
};





} /* namespace core */
} /* namespace pgen */




