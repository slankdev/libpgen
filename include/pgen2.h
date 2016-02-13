


#include <pgen2/types/data_container.h>

#include <pgen2/io/util.h>
#include <pgen2/io/arch.h>

#include <pgen2/core/address.h>
#include <pgen2/core/header.h>

// protocol class 
#include <pgen2/core/ethernet.h>




namespace pgen {
    using io::util::hex;
    


	// This component's designing 
    // will be do after designed core and io. 
    namespace module {}

    namespace core {
        /* 
         * Implemented class 
         *
         *      class header;
         *      class ethernet_header : header;
         */

        /* 
         * Implemented class 
         *
         *      class packet;
         */

        // class ipaddress;
        // class macaddress;
        // class port;
    }

    namespace io {
        /*
         * Implemented class
         *      
         *      class stream ;
         *      IO stream class with NETIF and FILES
         *
         */

        namespace util {
            /*
             * Implemented Functions
             *
             *      void hex(const void* buffer, size_t bufferlen);
             */
        }
        namespace netutil {}
        namespace arch {}
    }
	
	namespace types {
        /* 
         * Implemented class 
         *
         *      class data_container;
         */
    } 
}



namespace core {

	          
}
