
#pragma once

#include <stdint.h>
#include <iostream>


namespace pgen {



/**
 * This class provides MAC address functionality. 
 **/
class macaddress {
    public:
        static const size_t length = 6;             /**< Address length         */
        static const size_t strlength = 3*length-1; /**< Address't c_str length */ 
    private:

        uint8_t _raw[length];
        std::string _name;

        void _update_name();
        void  set_str(const std::string& str);

    public:
    
        macaddress();                         /**< default constructor                */
        macaddress(const macaddress& m);      /**< copy constructor                   */
        macaddress(const std::string& str);   /**< init address from std::string&     */
        macaddress(const char* str);          /**< init address from const char*      */
        void clear();                         /**< clear address "0.0.0.0"            */

        const uint8_t* raw() const;           /**< return address raw data            */
        const std::string str() const;        /**< get address string as std::string& */
        const std::string get_bender() const; /**< bender string, NOT IMPLEMENT       */

        /**
         * Set octet data.
         * @param[in] index address index. (1-6)
         * @param[in] oct octet data (0-255)
         **/
        void set_octet(int index, uint8_t oct);

        /**
         * Get octet data.
         * @param[in] index address index. (1-6)
         * @return octet data
         **/
        uint8_t get_octet(int index) const;
        

        macaddress& operator=(const macaddress& m);
        macaddress& operator=(const std::string& str);
        macaddress& operator=(const char* str);
        bool operator==(const macaddress& addr) const ;
        bool operator!=(const macaddress& addr) const ;

        /**
         * Set address by device name.
         * @param[in] ifname interface name. (ex, "eth0", "wlan0")
         **/
        void setbydev(const char* ifname);

        /**
         * Set address by array.
         * @param[in] array address array. (uint8_t[6])
         **/
        void setbyarray(const uint8_t array[6]);

};





} /* namespace pgen */
