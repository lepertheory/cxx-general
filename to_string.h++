/*****************************************************************************
 * to_string.h++
 *****************************************************************************
 * Convert a number to a string.
 *****************************************************************************/

// Include guard.
#if !defined(TOSTRING_buhu84hahasetkbx842)
  #define TOSTRING_buhu84hahasetkbx842

// STL includes.
  #include <sstream>
  #include <limits>

// Contain in namespace.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Convert the number to a string.
  template <class T> std::string to_string (T const& from) throw();
  
  // Convert a character to a string.
  std::string to_stringChr (char const from) throw();
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  template <> inline std::string to_string<char> (char const& from) throw() {
    std::ostringstream os;
    os << static_cast<int>(from);
    return os.str();
  }
  
  template <> inline std::string to_string<signed char> (signed char const& from) throw() {
    std::ostringstream os;
    os << static_cast<int>(from);
    return os.str();
  }
  
  template <> inline std::string to_string<unsigned char> (unsigned char const& from) throw() {
    std::ostringstream os;
    os << static_cast<unsigned int>(from);
    return os.str();
  }
  
  template <class T> inline std::string to_string (T const& from) throw() {
    std::ostringstream os;
    os.precision(std::numeric_limits<T>::digits10 + 1);
    os << from;
    return os.str();
  }
  
  inline std::string to_stringChr (char const from) throw() {
    std::ostringstream os;
    os << from;
    return os.str();
  }
  
}

#endif
