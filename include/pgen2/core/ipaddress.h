
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {



/**
 * This class provides IPv4 address functionality. 
 **/
class ipv4address {
    public:

        static const size_t length = 4;             /**< Address length         */
        static const size_t strlength = 3*length+3; /**< Address't c_str length */

    private:

        uint8_t _raw[length];
        std::string _name;

        void _update_name();
        void set_str(const std::string& str);

    public:

        ipv4address();                         /**< default constructor                */
        ipv4address(const ipv4address& i);     /**< copy constructor                   */
        ipv4address(const std::string& str);   /**< init address from std::string&     */
        ipv4address(const char* str);          /**< init address from const char*      */
        void clear();                          /**< clear address "0.0.0.0"            */

        const void* raw() const ;              /**< return address raw data            */
        const std::string& str() const ;       /**< get address string as std::string& */

        /**
         * Set octet data.
         * @param[in] index address index. (1-6)
         * @param[in] oct octet data (0-255)
         **/
        void     set_octet(int index, uint8_t oct);

        /**
         * Get octet data.
         * @param[in] index address index. (1-6)
         * @return octet data
         **/
        uint8_t  get_octet(int index) const ;

        ipv4address& operator=(const ipv4address& i);
        ipv4address& operator=(const std::string& str);
        ipv4address& operator=(const char* str);
        bool operator==(const ipv4address& addr) const ;
        bool operator!=(const ipv4address& addr) const ;

        /**
         * Set address by device name.
         * @param[in] ifname interface name. (ex, "eth0", "wlan0")
         **/
        void setbydev(const char* ifname);
        
        void setbyarray(const uint8_t array[4]);
        void copytoarray(uint8_t array[4]) const;


        // uint8_t& operator [] (int index); // for v4
        // int setmaskbydev(const char* ifname);
        // int setnetbydev(const char* ifname);
        // int maskby(const ipaddress& mask);
};


/**
 * This class provides IPv6 address functionality. 
 **/
class ipv6address {
    public:

        static const size_t length = 16;            /**< Address length         */
        static const size_t strlength = 2*length+7; /**< Address't c_str length */ 

    private:

        uint16_t _raw[length / sizeof(uint16_t)];
        std::string _name;

        void _update_name();
        void set_str(const std::string& str);

    public:

        ipv6address();                              /**< default constructor                */
        ipv6address(const ipv6address& i);          /**< copy constructor                   */
        ipv6address(const std::string& str);        /**< init address from std::string&     */
        ipv6address(const char* str);               /**< init address from const char*      */
        void clear();                               /**< clear address "0.0.0.0"            */

        const void* raw() const ;                   /**< return address raw data            */
        const std::string& str() const ;            /**< get address string as std::string& */

        /**
         * Set section data.
         * @param[in] index address index. (1-8)
         * @param[in] oct section data (0-255)
         **/
        void set_section(int index, uint16_t num);

        /**
         * Get octet data.
         * @param[in] index address index. (1-6)
         * @return octet data
         **/
        uint16_t get_section(int index) const ;   

        ipv6address& operator=(const ipv6address& i);
        ipv6address& operator=(const std::string& str);
        ipv6address& operator=(const char* str);
        bool operator==(const ipv6address& addr) const ;
        bool operator!=(const ipv6address& addr) const ;

        /**
         * Set address by device name.
         * @param[in] ifname interface name. (ex, "eth0", "wlan0")
         **/
        void setbydev(const char* ifname);

        void setbyarray(const uint16_t array[8]);
        void copytoarray(uint16_t array[8]) const;

        // uint8_t& operator [] (int index); // for v4
        // int setmaskbydev(const char* ifname);
        // int setnetbydev(const char* ifname);
        // int maskby(const ipaddress& mask);
};




} /* namespace pgen */

