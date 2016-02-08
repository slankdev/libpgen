
namespace pgen {
namespace core {



class ipaddr {
	private:
		uint32_t _rawv4;
		uint8_t _rawv6[16];
		std::string _name;
		bool _isV4; 
		
	public:

		ipaddr(bool isv4=true);
		ipaddr(const ipaddr& i);
		ipaddr(const std::string& str);
		void clear();
		
		void set_str(const std::string& str);
		const std::string& get_str() const ;

		void setOctet(int index, uint8_t oct);
		uint8_t getOctet(int index) const ;
		void setRaw4(uint32_t raw); 
		uint32_t getRaw() const ;

		int setipbydev(const char* ifname);
		int setmaskbydev(const char* ifname);
		int setnetbydev(const char* ifname);
		int maskby(const ipaddr& mask);

		ipaddr& operator=(const ipaddr& i);
		ipaddr& operator=(const std::string& str);
		bool operator==(const ipaddr& addr) const ;
		bool operator!=(const ipaddr& addr) const ;
};



class macaddr {
	private:
		uint8_t _raw[6];
		std::string _name;

	public:
	
		macaddr();
		macaddr(const macaddr& m);
		macaddr(const std::string& str);
		void clear(void);

		void set_str(const std::string& str);
		const std::string get_str() const ;
		const std::string get_bender() const ;

		void setOctet(int index, uint8_t oct);
		uint8_t getOctet(int index) const ;
		// void setRaw(uint48_t raw); 
		// uint48_t getRaw() const ;

		int setmacbyarry(const u_char* array);
		int setmacbydev(const char* ifname);

		macaddr& operator=(const macaddr& m);
		macaddr& operator=(const std::string& str);
		bool operator==(const macaddr& addr) const ;
		bool operator!=(const macaddr& addr) const ;
};



} /* namespace core */
} /* namespace pgen */
