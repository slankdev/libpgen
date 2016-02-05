
/*
 * implementation example of ethernet header class 
 * it use macaddr class in pgen version 1.0
 * I must reimplement macaddr class too. member function and member variable are not secure yet. 
 */

#include <stdio.h>
#include <pgen.h>

class header {
	private:
		macaddr _src;
		macaddr _dst;
		unsigned short _type;

	public:

		void print() ;

		/* getter */
		const macaddr& src() const;
		const macaddr& dst() const;

		/* setter */
		macaddr& src();
		macaddr& dst();

};
inline const macaddr& header::src() const {
	return _src;	
}
inline const macaddr& header::dst() const {
	return _dst;	
}
inline macaddr& header::src() {
	return _src;	
}
inline macaddr& header::dst() {
	return _dst;	
}



int main() {
	header eth;
	printf("eth.src: %s \n", eth.src().c_str());
	
	eth.src() = "11:22:33:44:55:66";
	printf("eth.src: %s \n", eth.src().c_str());
}
