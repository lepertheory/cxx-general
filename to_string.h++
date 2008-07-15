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
	// FIXME: All of these go out of scope before returning.
  
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
		static std::string buffer;
    std::ostringstream os;
    To_String_Util::CharCast<char, std::numeric_limits<char>::is_signed>::op(from, os);
		buffer = os.str();
    return buffer;
  }
  
  template <> inline std::string to_string<signed char> (signed char const& from) {
		static std::string buffer;
    std::ostringstream os;
    os << static_cast<int>(from);
		buffer = os.str();
    return buffer;
  }
  
  template <> inline std::string to_string<unsigned char> (unsigned char const& from) {
		static std::string buffer;
    std::ostringstream os;
    os << static_cast<unsigned int>(from);
		buffer = os.str();
    return buffer;
  }
  
  template <class T> inline std::string to_string (T const& from) {
		static std::string buffer;
    std::ostringstream os;
    os.precision(std::numeric_limits<T>::digits10 + 1);
    os << from;
		buffer = os.str();
    return buffer;
  }
  
  inline std::string to_stringChr (char const from) {
		static std::string buffer;
    std::ostringstream os;
    os << from;
		buffer = os.str();
    return buffer;
  }
  
}

#endif
