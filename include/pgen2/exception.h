

#pragma once

// class exception : std::exception {
//     private:
//         std::string str;
//     public:
//         explicit exception(const std::string& s="") : str(s) {} 
//         template<class T>
//         exception& operator<<(const T& t) {
//             std::ostringstream os;
//             os << " " << t ;
//             str += os.str();
//             return *this;
//         }
//         const char* what() const {
//             return str.c_str(); 
//         }
// };
//
//
//
