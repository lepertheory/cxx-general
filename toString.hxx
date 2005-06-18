/*****************************************************************************
 * toString.hxx
 *****************************************************************************
 * Convert a number to a string.
 *****************************************************************************/

// Include guard.
#if !defined(TOSTRING_buhu84hahasetkbx842)
  #define TOSTRING_buhu84hahasetkbx842

// STL includes.
  #include <sstream>

// Contain in namespace.
namespace DAC {
  
  // Convert the number to a string.
  #if defined(SAFEINT_3k54kbuihub7hbh0)
  template <class T> std::string toString (SafeInt<T> const& from);
  #endif
  template <class T> std::string toString (T          const& from);
  
  // Convert a character to a string.
  std::string toStringChr (char const from);
  
  #if defined(SAFEINT_3k54kbuihub7hbh0)
  template <class T> inline std::string toString (SafeInt<T> const& from) {
    return toString(static_cast<T>(from));
  }
  #endif
  
  template <> inline std::string toString<char> (char const& from) {
    std::ostringstream os;
    os << static_cast<int>(from);
    return os.str();
  }
  
  template <> inline std::string toString<signed char> (signed char const& from) {
    std::ostringstream os;
    os << static_cast<int>(from);
    return os.str();
  }
  
  template <> inline std::string toString<unsigned char> (unsigned char const& from) {
    std::ostringstream os;
    os << static_cast<unsigned int>(from);
    return os.str();
  }
  
  template <class T> inline std::string toString (T const& from) {
    std::ostringstream os;
    os.precision(std::numeric_limits<T>::digits10 + 1);
    os << from;
    return os.str();
  }
  
  inline std::string toStringChr (char const from) {
    std::ostringstream os;
    os << from;
    return os.str();
  }
  
}

#endif
