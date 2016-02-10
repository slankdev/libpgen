


#include <vector>
#include <stdio.h>
#include <stdint.h>



void func(const void* d, int len);


#include <vector>
#include <stdint.h>

    
/*
 * container class -- manage the byte array for libpgen version 2.0
 * 
 * Data Structure
 * 		
 * 	    0 1 ...                     128  ....                                                N
 * 		+----------------------------+--------------------------...--------------------------+
 * 		| 0101...                    |  001011...                                            |  
 * 		+----------------------------+--------------------------...--------------------------+
 * 		 <------Packet-Headers------> <------------------Packet-Contents-------------------->
 * 	                                 ^ 
 * 	                               pivot
 *
 *		Packet Headers		--		Packet's headers. For example, if it's ip packet class, 
 *									this field contain ethernet and ip header fields.
 *
 *		Packet Contents		--		Packet's Contents, For example, if it's ip packet class,
 *									this field contain ip's data.
 */
class container {
	private:
		std::vector<uint8_t> _vec;
		static const size_t _pivot = 128; /* [byte] */
	public:
		container(size_t size=128) { 
			_vec.resize(size, 0);
		}
		void set_size(int size) {
			_vec.resize(size, 0);
		}
		size_t get_size() const {
			return _vec.size();	
		}
		void insert_after(int index, uint8_t* buf, size_t buflen) {
			std::vector<uint8_t>::iterator it = _vec.begin();
			it += index;
			_vec.insert(it , buf, buf+buflen);
		}
		void insert_before(int index, uint8_t* buf, size_t buflen) {
			std::vector<uint8_t>::iterator it = _vec.begin();
			it += index - buflen;
			_vec.insert(it , buf, buf+buflen);
		}
		void write_after(int index, uint8_t* buf, size_t buflen) {
			std::vector<uint8_t>::iterator it = _vec.begin();
			it += index;
			for (int i=0; i<buflen; i++, it++) {
				*it = buf[i];
			}
		}
		void write_before(int index, uint8_t* buf, size_t buflen) {
			std::vector<uint8_t>::iterator it = _vec.begin();
			it += index - buflen;
			for (int i=0; i<buflen; i++, it++) {
				*it = buf[i];
			}
		}




		void insert_tail( uint8_t* buf, size_t buflen) {
			_vec.insert(_vec.end(), buf, buf+buflen);
		}

		void hex() const {
			func((void*)_vec.data(), _vec.size());
		}
};



int main() {

	container c(256);
	c.hex();

	uint8_t a[] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	uint8_t b[] = {1,2,3,4,5,6,7,8};
	c.write_after(128, (uint8_t*)b, sizeof(b));
	c.write_before(128, (uint8_t*)a, sizeof(a));

	c.hex();
	
}






void func(const void* d, int len){
	printf("hexdump len: %d \n", len);

	const uint8_t* data = (const uint8_t*)d;

	int row=0;
	int column=0;
	for(row=0; (row+column)<16+len; row+=16){
		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf("   ");
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
			}else{
				if((row+column)%16 == 0) printf("%04x:    ", row+column);
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
				printf("%02x ", data[row+column]);
			}
		}

		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf(" ");
			}else{
				if((row+column)%16 == 0) 
					printf("  ");
				if((row+column)%8 == 0 && (row+column)%16!=0) 
					printf("  ");
				
				if(0x20<=data[row+column] && data[row+column]<=0x7E)
					printf("%c", data[row+column]);
				else
					printf(".");
				
				if((row+column+1)%16 == 0)	
					printf("\n");
			}
		}
	}
	printf("\n");
}

