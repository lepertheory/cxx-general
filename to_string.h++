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
  #include <string>

// Contain in namespace.
namespace DAC {
  
  /***************************************************************************/
  // Types
  
  // Helper class for char conversion.
  namespace To_String_Util {
    template <class T, bool> class CharCast;
    template <class T> class CharCast<T, false> { public: static std::ostringstream& op (T const& from, std::ostringstream& to); };
    template <class T> class CharCast<T, true > { public: static std::ostringstream& op (T const& from, std::ostringstream& to); };
  }
  
  /***************************************************************************/
  // Functions.
  
  // Convert the number to a string.
  template <class T> std::string to_string (T const& from);
  
  // Convert a character to a string.
  std::string to_stringChr (char const from);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  namespace To_String_Util {
    template <class T> inline std::ostringstream& CharCast<T, false>::op (T const& from, std::ostringstream& to) {
      to << static_cast<unsigned int>(from);
      return to;
    }
    template <class T> inline std::ostringstream& CharCast<T, true >::op (T const& from, std::ostringstream& to) {
      to << static_cast<int>(from);
      return to;
    }
  }
  
  template <> inline std::string to_string<char> (char const& from) {
    std::ostringstream os;
    To_String_Util::CharCast<char, std::numeric_limits<char>::is_signed>::op(from, os);
    return os.str();
  }
  
  template <> inline std::string to_string<signed char> (signed char const& from) {
    std::ostringstream os;
    os << static_cast<int>(from);
    return os.str();
  }
  
  template <> inline std::string to_string<unsigned char> (unsigned char const& from) {
    std::ostringstream os;
    os << static_cast<unsigned int>(from);
    return os.str();
  }
  
  template <class T> inline std::string to_string (T const& from) {
    std::ostringstream os;
    os.precision(std::numeric_limits<T>::digits10 + 1);
    os << from;
    return os.str();
  }
  
  inline std::string to_stringChr (char const from) {
    std::ostringstream os;
    os << from;
    return os.str();
  }
  
}

#endif
