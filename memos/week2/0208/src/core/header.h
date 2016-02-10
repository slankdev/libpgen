


namespace pgen {
namespace core {







// class test-protocol  {
// 	private: 
// 		#<{(| protocol member |)}>#
// 		#<{(|
// 		 * If protocol has extension data (ex, tcp and udp.. ), 
// 		 * this class manages that data as private mamber variable.
// 		 |)}>#
// 	public:
// 		#<{(| protocol member's setter   |)}>#
// 		#<{(| protocol ext-data's setter |)}>#
// 		#<{(| protocol member's getter   |)}>#
// 		#<{(| protocol ext-data's getter |)}>#
// 		size_t write_header(void* buffer, size_t buffer_len);
// 		size_t read_header(const void* buffer, size_t buffer_len);
// }



class ethernet_header {
	private:
		macaddr _dst;
		macaddr _src;
		bit16   _type;

	public:
		const macaddr& src() const;
		const macaddr& dst() const;
		macaddr& src();
		macaddr& dst();

		size_t write_header(void* buffer, size_t buffer_len);
		size_t read_header(const void* buffer, size_t buffer_len);

};



class ip_header {
	private:
		uint8_t    _hlen:4;
		uint8_t    _tos;
		uint16_t   _tot_len;
		uint16_t   _id
		uint16_t   _frag_off;
		uint8_t    _ttl;
		uint8_t    _protocol;
		uint16_t   _check;
		ipaddr     _src;
		ipaddr     _dst;     

	public:
		const uint8_t&    hlen() const ;    
		const uint8_t&    tos() const ;
		const uint16_t&   tot_len() const ;
		const uint16_t&   id() const;
		const uint16_t&   frag_off() const ;
		const uint8_t&    ttl() const ;
		const uint8_t&    protocol() const ;
		const uint16_t&   check() const ;
		const ipaddr&     src() const ;
		const ipaddr&     dst() const ;     
	
		uint8_t&    hlen();    
        uint8_t&    tos();
        uint16_t&   tot_len();
        uint16_t&   id();
        uint16_t&   frag_off();
        uint8_t&    ttl();
        uint8_t&    protocol();
        uint16_t&   check();
		ipaddr&     src();
		ipaddr&     dst();

		size_t write_header(void* buffer, size_t buffer_len);
		size_t read_header(const void* buffer, size_t buffer_len);
};





} /* namespace core */
} /* namespace pgen */




